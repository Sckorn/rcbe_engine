#ifndef RCBE_ENGINE_GLRENDERER_HPP
#define RCBE_ENGINE_GLRENDERER_HPP

#include <memory>
#include <future>

#include <rcbe-engine/datamodel/rendering/renderer_config.hpp>
#include <rcbe-engine/datamodel/geometry/Mesh.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>

namespace rcbe::rendering {
class GLRendererImplementation;

class GLRenderer
{
public:
    GLRenderer() = delete;
    GLRenderer(renderer_config &&config, const std::shared_ptr<RenderingContext>& context);

    ~GLRenderer();

    GLRenderer(const GLRenderer &other) = delete;
    GLRenderer &operator=(const GLRenderer &other) = delete;

    GLRenderer(GLRenderer &&other) = default;
    GLRenderer &operator=(GLRenderer &&other) = default;

    [[nodiscard]]bool running() const;

    [[nodiscard]]const renderer_config &config() const;

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
private:
    std::unique_ptr<GLRendererImplementation> impl_;
};

using GLRendererPtr = std::unique_ptr<GLRenderer>;
using GLRendererConstPtr = std::unique_ptr<const GLRenderer>;

GLRendererPtr make_renderer_ptr(renderer_config &&config, const std::shared_ptr<RenderingContext>& context);
}

#endif //RCBE_ENGINE_GLRENDERER_HPP
