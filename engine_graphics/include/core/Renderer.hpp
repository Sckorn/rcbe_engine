#ifndef RCBE_GRAPHICS_ENGINE_RENDERER_H
#define RCBE_GRAPHICS_ENGINE_RENDERER_H

#include <memory>
#include <atomic>
#include <mutex>
#include <future>
#include <type_traits>
#include <functional>

#include <data_types/math/Vector.hpp>
#include <data_types/visual/RGBAColor.hpp>
#include <data_types/geometry/Mesh.hpp>

namespace rcbe::core::rendering
{

class RendererImplementation;

struct RendererConfig
{
    std::string window_caption = "RCBE Engine";
    size_t initial_width = 1024;
    size_t initial_height = 768;
    bool resizable = true;
    rcbe::math::Vector2d initial_position = {initial_width / 2, initial_height / 2};
    rcbe::visual::RGBAColor clear_color = { 0., 0., 0., 1. };
    rcbe::core::EngineScalar camera_zoom = -20.0f;
};

class Renderer
{
// static part
private:
    static std::unique_ptr<RendererImplementation> _renderer_instance;
    static std::atomic<bool> _initialized;
    static std::atomic<size_t> _instance_counter;
    static std::mutex _op_mutex;

// static part
public:
    Renderer() = delete;
    Renderer(RendererConfig &&config);
    
    ~Renderer();

    Renderer(const Renderer &other) = delete;
    Renderer &operator=(const Renderer &other) = delete;

    Renderer(Renderer &&other) = default;
    Renderer &operator=(Renderer &&other) = default;

    const RendererConfig &config() const;
    //TODO: figure out how to return a future
    //std::future<typename std::result_of<std::function<void()>>::type> launch();
    void launch();
    void addObject(rcbe::geometry::Mesh &&mesh);
};

}

#endif //RCBE_GRAPHICS_ENGINE_RENDERER_H