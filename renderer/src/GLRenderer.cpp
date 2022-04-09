#include "GLRenderer.hpp"

#include <mutex>
#include <cmath>
#include <atomic>
#include <random>
#include <ranges>
#include <unordered_map>

#include <rcbe-engine/core/gl_extensions.hpp>

#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/datamodel/rendering/ShaderProgram.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/matrix_helpers.hpp>
#include <rcbe-engine/datamodel/rendering/matrix_helpers.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include <rcbe-engine/datamodel/rendering/Material.hpp>

#include <boost/log/trivial.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>

namespace {
static constexpr GLint ignore_errors[] = { GL_INVALID_VALUE };

bool isErrorIgnored(const GLint errnum) {
    return std::ranges::any_of(std::begin(ignore_errors), std::end(ignore_errors), [&](auto i) {
        return i == errnum;
    });
}
}

namespace rdmn::render {

GLRenderer::GLRenderer(
        rcbe::rendering::renderer_config &&config,
        const std::shared_ptr<rcbe::rendering::RenderingContext> &context
)
:
 config_ { std::move(config) }
 , rendering_context_ { context }
{
    if (rendering_context_ == nullptr)
        throw std::runtime_error("RenderingContext is null");

    if (rendering_context_->getGlxContext() == nullptr)
        throw std::runtime_error("GLX context is null");
}

GLRenderer::~GLRenderer() {
    stop();
}

void GLRenderer::setPerspective(const double fov, const double aspect, const double near, const double far) {
    auto field_height = std::tan( (fov / 360 ) * M_PI ) * near;
    auto field_width = field_height * aspect;

    glFrustum(-field_width, field_width, -field_height, field_height, near, far);
}


void GLRenderer::initGL() {
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    const auto& bg_color = rendering_context_->getBackgroundColor();
    glClearColor(bg_color.r(), bg_color.g(), bg_color.b(), bg_color.a());                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}

void GLRenderer::initLights() {
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 1, 0}; // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}

void GLRenderer::waitRendererReady() {
    std::unique_lock ul { ready_mutex_ };
    if (this->ready_) return;
    renderer_ready_cv_.wait(ul, [this]() { return this->ready_; });
}

void GLRenderer::onStop(rcbe::rendering::RendererStopHandlerType&& handler) {
    stop_handler_ = std::move(handler);
}

void GLRenderer::reshape() {
    const auto &dimensions = rendering_context_->getWindowDimensions();
    glViewport(0, 0, dimensions.width, dimensions.height);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    setPerspective(static_cast<float>(rendering_context_->getFov()), static_cast<float>(dimensions.width) / static_cast<float>(dimensions.height), 0.1f, 100.0f);

    const auto trn = rendering_context_->getSceneTransformColumnMajor();
    glMultMatrixd(static_cast<const GLdouble *>(trn.getRaw()));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    {
        std::lock_guard lg { ready_mutex_ };
        if (!ready_)
            ready_ = true;
    }
    renderer_ready_cv_.notify_all();
}

bool GLRenderer::running() const {
    return running_.load();
}

void GLRenderer::start() {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true)) return;

    try {
        ticker_ = std::make_shared<rcbe::core::Ticker>(std::chrono::milliseconds(1), [this]() {
            renderFrame();
        });

        rendering_context_->startTime(std::chrono::steady_clock::now());

        ticker_->wait();

    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Exception in main rendering routine " << e.what();
        stop();
        throw;
    }

    objects_.clear();
}

void GLRenderer::stop() {
    bool expected = true;
    if (!running_.compare_exchange_strong(expected, false)) return;

    ticker_->stop();
    ticker_->wait();

    if (stop_handler_)
        stop_handler_(rendering_context_);
}

void GLRenderer::addObject(rcbe::core::CoreObject &&object) {
    if (!object.hasComponent<rcbe::rendering::Material>() && !object.hasComponent<rcbe::geometry::Mesh>()) {
        BOOST_LOG_TRIVIAL(error) << "Can't add object to renderer!";
        return;
    }

    {
        std::lock_guard lg { changed_mutex_ };
        const auto id = object.id();
        objects_.insert_or_assign(id, std::move(object));
    }

    changed_.store(true);
}

void GLRenderer::drawBuffers(
        const rcbe::rendering::VertexBufferObject& vbo,
        const rcbe::rendering::IndexBufferObject& ibo
) {
    drawBuffers(vbo, ibo, {});
}

void GLRenderer::drawBuffers(
        const rcbe::rendering::VertexBufferObject& vbo,
        const rcbe::rendering::IndexBufferObject& ibo,
        const std::unordered_map<size_t, rcbe::core::CoreObject> &objects
) {
    const std::vector<DrawBuffersHandlerType> draw_impls = {
            {[this](
                    const rcbe::rendering::VertexBufferObject &vbo,
                    const rcbe::rendering::IndexBufferObject &ibo,
                    const std::unordered_map<size_t, rcbe::core::CoreObject> &objects) {
                vbo.enableState();

                // before draw, specify vertex arrays
                glNormalPointer(GL_FLOAT, 0, vbo.normalsData());
                glColorPointer(3, GL_FLOAT, 0, vbo.colorsData());
                glVertexPointer(3, GL_FLOAT, 0, vbo.verticesData());

                glDrawElements(GL_TRIANGLES,            // primitive type
                               ibo.size(),                      // # of indices
                               GL_UNSIGNED_INT,         // data type
                               (void*)ibo.data());         // ptr to indices

                vbo.disableState();
            }},
            {[this](
                    const rcbe::rendering::VertexBufferObject &vbo,
                    const rcbe::rendering::IndexBufferObject &ibo,
                    const std::unordered_map<size_t, rcbe::core::CoreObject> &shader_prog) {
                /// TODO: renderer is hardware, this only changes wheather shaders are used or not.
                bool hardware_renderer = (config_.renderer_type == rcbe::rendering::RendererType ::hardware);
                BOOST_LOG_TRIVIAL(debug) << "Renderer is " << ((hardware_renderer) ? "hardware." : "software.");
                // bind VBOs with IDs and set the buffer offsets of the bound VBOs
                // When buffer object is bound with its ID, all pointers in gl*Pointer()
                // are treated as offset instead of real pointer.

                if (hardware_renderer) {
                    vbo.vao().bind();
                    ibo.ebo().bind();
                } else {
                    vbo.bind();
                    ibo.bind();
                }

                // enable vertex arrays
                if (!hardware_renderer)
                    vbo.enableState();

                auto v = rendering_context_->computeDeltaTime();
                float greenValue = std::sin(v) + 0.5f;

                if (hardware_renderer) {
                    // TODO: add orthographic projection computation and test it later
                    //auto perspective = glm::ortho(0.0f, static_cast<float>(dimensions.width), 0.0f, static_cast<float>(dimensions.height), 0.1f, 100.0f);

                    auto perspective = rdmn::render::makePerspectiveMatrix(
                            0.1,
                            100.0,
                            rendering_context_->getFov(),
                            rendering_context_->getWindowDimensions()
                            );

                    size_t offset = 0;
                    for (const auto &[id, object] : objects_) {
                        const auto mesh_comp = object.getComponent<rcbe::geometry::Mesh>();
                        if (!mesh_comp)
                            throw std::runtime_error("Can't retrieve mesh component from rendering object!");

                        const auto &mesh = mesh_comp->as<rcbe::geometry::Mesh>();
                        const auto indices_to_draw = mesh.facetsSize() * 3;

                        const auto material_comp = object.getComponent<rcbe::rendering::Material>();
                        if (!material_comp)
                            throw std::runtime_error("Can't retrieve material component from rendering object!");

                        const auto &material = material_comp->as<rcbe::rendering::Material>();
                        material.apply();

                        const auto trn = rendering_context_->getSceneTransformColumnMajor();
                        material.getShaderProgram()->setFloat("customGreen", greenValue);

                        auto trnf = trn.convertValuesTo<float>();
                        material.getShaderProgram()->setMatrix("view", trnf);

                        auto persp = rcbe::math::MatrixColumnMajorAdaptor<float>(perspective);

                        material.getShaderProgram()->setMatrix("perspective", persp);

                        const auto &mesh_trn = mesh.getTransform().matrix();
                        auto mesh_trnf = rcbe::math::MatrixColumnMajorAdaptor<float>(mesh_trn);
                        material.getShaderProgram()->setMatrix("model", mesh_trnf);
                        glDrawArrays( GL_TRIANGLES, offset, indices_to_draw);
                        offset += indices_to_draw;
                    }
                } else {
                    glDrawElements(GL_TRIANGLES,            // primitive type
                                   ibo.size(),              // # of indices
                                   GL_UNSIGNED_INT,         // data type
                                   nullptr);               // ptr to indices
                    glDrawArrays( GL_TRIANGLES, 0, ibo.size());
                }

                if (!hardware_renderer)
                    vbo.disableState();

                // it is good idea to release VBOs with ID 0 after use.
                // Once bound with 0, all pointers in gl*Pointer() behave as real
                // pointer, so, normal vertex array operations are re-activated
                if (hardware_renderer) {
                    vbo.vao().unbind();
                    ibo.ebo().unbind();
                } else {
                    vbo.unbind();
                    ibo.unbind();
                }
            }}
    };

    draw_impls[vbo_supported_](vbo, ibo, objects);
}

void GLRenderer::renderFrame() {
    [[maybe_unused]] const auto delta_time = rendering_context_->computeDeltaTime();

    rendering_context_->glContextFromThis();

    const auto& color = rendering_context_->getBackgroundColor();

    GLExtensions& ext = GLExtensions::getInstance();
    vbo_supported_ = ext.isSupported("GL_ARB_vertex_buffer_object");

    initGL();

    for (const auto &[id, object] : objects_) {
        const auto material_component = object.getComponent<rcbe::rendering::Material>();
        if (material_component) {
            const auto &material = material_component->as<rcbe::rendering::Material>();
            if (material.isDeferred())
                material.initializeDeferredMaterial();
        }
    }

    BOOST_LOG_TRIVIAL(debug) << "VBO is " << (vbo_supported_ ? "" : "not ") << "supported";

    rcbe::core::GLErrorProcessor error_processor {};

    {
        std::lock_guard lg {reshape_mutex_};
        reshape();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    /// main rendering start
    if (!objects_.empty()) {
        /// TODO: renderer is hardware, this only changes wheather shaders are used or not.
        bool hardware_renderer = config_.renderer_type == rcbe::rendering::RendererType ::hardware;

        std::vector<rcbe::geometry::Mesh> meshes;
        meshes.reserve(objects_.size());

        for (const auto &[id, object] : objects_) {
            auto mesh_component = object.getComponent<rcbe::geometry::Mesh>();
            auto mesh = mesh_component->as<rcbe::geometry::Mesh>();
            meshes.push_back(mesh);
        }

        {
            std::lock_guard lg { changed_mutex_ };
            if (changed_) {
                vertex_buffer_ = std::make_unique<rcbe::rendering::VertexBufferObject>(meshes, hardware_renderer);
                index_buffer_ = std::make_unique<rcbe::rendering::IndexBufferObject>(meshes, *vertex_buffer_, hardware_renderer);
                changed_ = false;
            }
        }

        BOOST_LOG_TRIVIAL(debug) << "Trying to invoke drawBuffers";
        drawBuffers(*vertex_buffer_, *index_buffer_, objects_);
    }
    /// end rendering start

    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        if (!isErrorIgnored(error)) {
            BOOST_LOG_TRIVIAL(error) << error_processor(error);
            BOOST_LOG_TRIVIAL(error) << "error hex code " << std::hex << error;
        }
    }

    glXSwapBuffers(rendering_context_->getDisplay(), rendering_context_->getDrawable());
}

const rcbe::rendering::renderer_config& GLRenderer::getConfig() const noexcept {
    return config_;
}
}
