#include <rcbe-engine/renderer/GLRenderer.hpp>

#include <mutex>
#include <cmath>
#include <atomic>

#include <rcbe-engine/core/gl_extensions.hpp>

#include <rcbe-engine/datamodel/rendering/BufferObject.hpp>
#include <rcbe-engine/ticker/Ticker.hpp>

#include <boost/log/trivial.hpp>

namespace rcbe::rendering {
class GLRendererImplementation {
public:
    GLRendererImplementation(renderer_config &&config, const std::shared_ptr<RenderingContext>& context);
    ~GLRendererImplementation();

    [[nodiscard]]bool running() const;

    void start();
    void stop();

    [[nodiscard]] const renderer_config& getConfig() const noexcept;

    void addObject(rcbe::geometry::Mesh &&object);

    void reshapeWindow();

    void onStop(RendererStopHandlerType&& handler);

private:
    std::chrono::microseconds renderFrame();

    void initGL();

    void initLights();

    void setPerspective(const double fov, const double aspect, const double near, const double far);

    void drawBuffers(const VertexBufferObject& vbo, const IndexBufferObject& ibo);

    renderer_config config_;
    std::shared_ptr<RenderingContext> rendering_context_;
    
    std::mutex changed_mutex_;
    std::mutex reshape_mutex_;
    std::atomic_bool running_ = false;
    bool vbo_supported_ = false;

    std::vector<rcbe::geometry::Mesh> meshes_;

    std::vector<VertexBufferObject> vertex_buffer_;
    std::vector<IndexBufferObject> index_buffer_;

    bool changed_ = false;

    RendererStopHandlerType stop_handler_;

    std::shared_ptr<rcbe::core::Ticker> ticker_;
};

GLRendererImplementation::GLRendererImplementation(renderer_config &&config, const std::shared_ptr<RenderingContext>& context)
:
 config_ { std::move(config) }
 , rendering_context_ { context }
{
    if (rendering_context_ == nullptr)
        throw std::runtime_error("RenderingContext is null");

    if (rendering_context_->getGlxContext() == nullptr)
        throw std::runtime_error("GLX context is null");
}

GLRendererImplementation::~GLRendererImplementation() {
    stop();
}

void GLRendererImplementation::setPerspective(const double fov, const double aspect, const double near, const double far) {
    auto field_height = std::tan( (fov / 360 )* M_PI ) * near;
    auto field_width = field_height * aspect;

    glFrustum(-field_width, field_width, -field_height, field_height, near, far);
}


void GLRendererImplementation::initGL() {
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

void GLRendererImplementation::initLights() {
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

void GLRendererImplementation::onStop(RendererStopHandlerType&& handler) {
    stop_handler_ = std::move(handler);
}

void GLRendererImplementation::reshapeWindow() {
    const auto& dimensions = rendering_context_->getWindowDimensions();
    glViewport(0, 0, dimensions.width, dimensions.height);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    setPerspective(rendering_context_->getZoom(), dimensions.width / dimensions.height, 0.1f, 100.0f);

    const auto trn = rendering_context_->getTransformColumnMajor();
    glMultMatrixd(static_cast<const GLdouble *>(trn.getRaw()));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool GLRendererImplementation::running() const {
    return running_.load();
}

void GLRendererImplementation::start() {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true)) return;

    try {
        ticker_ = std::make_shared<rcbe::core::Ticker>(std::chrono::milliseconds(1), [this]() {
            rendering_context_->setCurrentTime(renderFrame());
        });

        ticker_->wait();

    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Exception in main rendering routine " << e.what();
        stop();
    }

    meshes_.clear();
    index_buffer_.clear();
    vertex_buffer_.clear();
}

void GLRendererImplementation::stop() {
    bool expected = true;
    if (!running_.compare_exchange_strong(expected, false)) return;

    ticker_->stop();
    ticker_->wait();

    if (stop_handler_)
        stop_handler_(rendering_context_);
}

void GLRendererImplementation::addObject(rcbe::geometry::Mesh &&object) {
    std::lock_guard lg { changed_mutex_ };
    meshes_.push_back(std::move(object));
    changed_ = true;
}

void GLRendererImplementation::drawBuffers(const VertexBufferObject& vbo, const IndexBufferObject& ibo) {
    const std::vector<std::function<void(const VertexBufferObject&, const IndexBufferObject&)>> draw_impls = {
            {[this](const VertexBufferObject& vbo, const IndexBufferObject& ibo) {
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
            {[this](const VertexBufferObject& vbo, const IndexBufferObject& ibo) {
                // bind VBOs with IDs and set the buffer offsets of the bound VBOs
                // When buffer object is bound with its ID, all pointers in gl*Pointer()
                // are treated as offset instead of real pointer.
                vbo.bind();
                ibo.bind();

                // enable vertex arrays
                vbo.enableState();

                // before draw, specify vertex and index arrays with their offsets
                glNormalPointer(GL_FLOAT, 0, (void*) vbo.vertsByteSize());
                glColorPointer(3, GL_FLOAT, 0, (void*)(vbo.vertsByteSize() + vbo.normsByteSize()));
                glVertexPointer(3, GL_FLOAT, 0, 0);
                glDrawElements(GL_TRIANGLES,            // primitive type
                               ibo.size(),//RendererImplementation::_indices.size(),                      // # of indices
                               GL_UNSIGNED_INT,         // data type
                               (void*)0);               // ptr to indices
                glDrawArrays( GL_TRIANGLES, 0, vbo.vertsByteSize() + vbo.normsByteSize() + vbo.colorsByteSize());

                vbo.disableState();

                // it is good idea to release VBOs with ID 0 after use.
                // Once bound with 0, all pointers in gl*Pointer() behave as real
                // pointer, so, normal vertex array operations are re-activated
                vbo.unbind();
                ibo.unbind();
            }}
    };

    draw_impls[vbo_supported_](vbo, ibo);
}

std::chrono::microseconds GLRendererImplementation::renderFrame() {
    auto start = std::chrono::steady_clock::now();
    rendering_context_->glContextFromThis();

    const auto& color = rendering_context_->getBackgroundColor();

    GLExtensions& ext = GLExtensions::getInstance();
    vbo_supported_ = ext.isSupported("GL_ARB_vertex_buffer_object");

    initGL();

    BOOST_LOG_TRIVIAL(debug) << "VBO is " << (vbo_supported_ ? "" : "not ") << "supported";

    core::GLErrorProcessor error_processor {};

    {
        std::lock_guard lg {reshape_mutex_};
        reshapeWindow();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    /// main rendering start
    if (!meshes_.empty()) {
        if (vertex_buffer_.empty() || changed_) {
            std::lock_guard lg { changed_mutex_ };
            vertex_buffer_.emplace_back(meshes_);
        }

        const auto& vbo = vertex_buffer_.back();

        if (index_buffer_.empty() || changed_) {
            std::lock_guard lg { changed_mutex_ };
            index_buffer_.emplace_back(meshes_, vbo);
            changed_ = false;
        }

        const auto& ibo = index_buffer_.back();

        drawBuffers(vbo, ibo);
    }
    /// end rendering start

    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        BOOST_LOG_TRIVIAL(error) << error_processor(error);
        BOOST_LOG_TRIVIAL(error) << "error hex code " << std::hex << error;
    }

    glXSwapBuffers(rendering_context_->getDisplay(), rendering_context_->getDrawable());
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

const renderer_config& GLRendererImplementation::getConfig() const noexcept {
    return config_;
}

GLRenderer::GLRenderer(renderer_config &&config, const std::shared_ptr<RenderingContext>& context)
:
impl_ { std::make_unique<GLRendererImplementation>(std::move(config), context) }
{

}

GLRenderer::~GLRenderer() {

}

const renderer_config &GLRenderer::config() const noexcept {
    return impl_->getConfig();
}

void GLRenderer::start() {
    impl_->start();
}

void GLRenderer::stop() {
    impl_->stop();
}

void GLRenderer::addObject(rcbe::geometry::Mesh &&mesh) {
    impl_->addObject(std::move(mesh));
}

void GLRenderer::reshape() {
    impl_->reshapeWindow();
}

void GLRenderer::onStop(RendererStopHandlerType&& handler) {
    impl_->onStop(std::move(handler));
}

bool GLRenderer::running() const {
    return impl_->running();
}

GLRendererPtr make_renderer_ptr(renderer_config &&config, const std::shared_ptr<RenderingContext>& context) {
    return std::make_unique<GLRenderer>(std::move(config), context);
}
}
