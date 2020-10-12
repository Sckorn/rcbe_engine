#ifndef RCBE_ENGINE_GLRENDERER_HPP
#define RCBE_ENGINE_GLRENDERER_HPP

#include <memory>
#include <future>

#include <data_types/rendering/RendererConfig.hpp>
#include <data_types/geometry/Mesh.hpp>
#include <data_types/math/Vector.hpp>

#include <data_types/rendering/RenderingContext.hpp>

namespace rcbe::rendering {
class GLRendererImplementation;

class GLRenderer
{
public:
    GLRenderer() = delete;
    GLRenderer(RendererConfig &&config, const std::shared_ptr<RenderingContext>& context);

    ~GLRenderer();

    GLRenderer(const GLRenderer &other) = delete;
    GLRenderer &operator=(const GLRenderer &other) = delete;

    GLRenderer(GLRenderer &&other) = default;
    GLRenderer &operator=(GLRenderer &&other) = default;

    [[nodiscard]]bool running() const;

    [[nodiscard]]const RendererConfig &config() const;

    decltype(auto) start_async() {
        return std::async(std::launch::async, [this]() {
            start();
        });
    }
    void start();
    void stop();
    void add_object(rcbe::geometry::Mesh &&mesh);

    void reshape();

    void on_stop(renderer_stop_handler_t&& handler);

    void translate_scene(const rcbe::math::Vector3d& t) const;
    void rotate_scene(const rcbe::math::Vector3d& t, rcbe::core::EngineScalar angle_deg) const;

private:
    std::unique_ptr<GLRendererImplementation> impl_;
};

using GLRendererPtr = std::unique_ptr<GLRenderer>;
using GLRendererConstPtr = std::unique_ptr<const GLRenderer>;

GLRendererPtr make_renderer_ptr(RendererConfig &&config, const std::shared_ptr<RenderingContext>& context);
}

#endif //RCBE_ENGINE_GLRENDERER_HPP
