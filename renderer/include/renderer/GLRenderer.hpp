#ifndef RCBE_ENGINE_GLRENDERER_HPP
#define RCBE_ENGINE_GLRENDERER_HPP

#include <memory>
#include <future>

#include <data_types/rendering/RendererConfig.hpp>
#include <data_types/geometry/Mesh.hpp>

#include <core/XWWindow.hpp>

namespace rcbe::rendering {
class GLRendererImplementation;

class GLRenderer
{
public:
    GLRenderer() = delete;
    GLRenderer(RendererConfig &&config, const core::WindowPtr& window );

    ~GLRenderer();

    GLRenderer(const GLRenderer &other) = delete;
    GLRenderer &operator=(const GLRenderer &other) = delete;

    GLRenderer(GLRenderer &&other) = default;
    GLRenderer &operator=(GLRenderer &&other) = default;

    [[nodiscard]]const RendererConfig &config() const;

    decltype(auto) start_async() {
        return std::async(std::launch::async, [this]() {
            start();
        });
    }
    void start();
    void stop();
    void add_object(rcbe::geometry::Mesh &&mesh);

private:
    std::unique_ptr<GLRendererImplementation> impl_;
};
}

#endif //RCBE_ENGINE_GLRENDERER_HPP
