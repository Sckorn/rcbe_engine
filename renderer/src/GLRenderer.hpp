#ifndef RDMN_ENGINE_GLRENDERER_HPP
#define RDMN_ENGINE_GLRENDERER_HPP

#include <future>
#include <memory>

#include <rcbe-engine/datamodel/core/CoreObject.hpp>
#include <rcbe-engine/datamodel/geometry/Mesh.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/rendering/BufferObject.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>
#include <rcbe-engine/datamodel/rendering/renderer_config.hpp>
#include <rcbe-engine/ticker/Ticker.hpp>

namespace rdmn::render {

class GLRenderer {
public:

    using DrawBuffersHandlerType = std::function<
        void(
            const rcbe::rendering::VertexBufferObject &,
            const rcbe::rendering::IndexBufferObject &,
            const std::unordered_map<size_t, rcbe::core::CoreObject> &)>;

    GLRenderer() = delete;
    GLRenderer(
        rcbe::rendering::renderer_config &&config,
        const std::shared_ptr<rcbe::rendering::RenderingContext> &context);
    ~GLRenderer();

    [[nodiscard]] bool running() const;

    void start();
    void stop();

    [[nodiscard]] const rcbe::rendering::renderer_config &getConfig() const noexcept;

    void addObject(rcbe::core::CoreObject &&object);

    void reshape();

    void onStop(rcbe::rendering::RendererStopHandlerType &&handler);

    void waitRendererReady();

private:

    void renderFrame();

    void initGL();

    void initLights();

    void setPerspective(const double fov, const double aspect, const double near, const double far);

    void drawBuffers(const rcbe::rendering::VertexBufferObject &vbo, const rcbe::rendering::IndexBufferObject &ibo);
    void drawBuffers(
        const rcbe::rendering::VertexBufferObject &vbo,
        const rcbe::rendering::IndexBufferObject &ibo,
        const std::unordered_map<size_t, rcbe::core::CoreObject> &objects);

    rcbe::rendering::renderer_config config_;
    std::shared_ptr<rcbe::rendering::RenderingContext> rendering_context_;

    std::mutex changed_mutex_;
    std::mutex reshape_mutex_;
    std::mutex ready_mutex_;
    std::atomic_bool running_ = false;
    bool ready_ = false;
    std::condition_variable renderer_ready_cv_;
    bool vbo_supported_ = false;

    std::unordered_map<size_t, rcbe::core::CoreObject> objects_;

    std::unique_ptr<rcbe::rendering::VertexBufferObject> vertex_buffer_ = nullptr;
    std::unique_ptr<rcbe::rendering::IndexBufferObject> index_buffer_ = nullptr;

    std::atomic_bool changed_ = false;

    rcbe::rendering::RendererStopHandlerType stop_handler_;

    std::shared_ptr<rcbe::core::Ticker> ticker_;
};

using GLRendererPtr = std::unique_ptr<GLRenderer>;
using GLRendererConstPtr = std::unique_ptr<const GLRenderer>;
}// namespace rdmn::render

#endif//RDMN_ENGINE_GLRENDERER_HPP
