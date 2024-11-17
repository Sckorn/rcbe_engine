#ifndef RDMN_ENGINE_RENDERER_HPP
#define RDMN_ENGINE_RENDERER_HPP

#include <future>
#include <memory>

#include <rcbe-engine/datamodel/core/CoreObject.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>
#include <rcbe-engine/datamodel/rendering/renderer_config.hpp>

namespace rdmn::render {
class RendererImpl;

class R_PUBLIC_API Renderer {
public:

    Renderer() = delete;

    Renderer(rcbe::rendering::renderer_config &&config,
             const std::shared_ptr<rcbe::rendering::RenderingContext> &context);

    ~Renderer();

    [[nodiscard]] bool running() const;

    decltype(auto) startAsync() {
        return std::async(std::launch::async, [this]() {
            start();
        });
    }

    void waitRendererReady();

    void start();

    void stop();

    [[nodiscard]] const rcbe::rendering::renderer_config &getConfig() const noexcept;

    void reshape();

    void addObject(rcbe::core::CoreObject &&object);
    void addObjects(std::vector<rcbe::core::CoreObject> &&object);

private:

    std::unique_ptr<RendererImpl> impl_;
};

using RendererPtr = std::unique_ptr<Renderer>;
using RendererConstPtr = std::unique_ptr<const Renderer>;

R_PUBLIC_API RendererPtr make_renderer_ptr(
    rcbe::rendering::renderer_config &&config,
    const std::shared_ptr<rcbe::rendering::RenderingContext> &context);

R_PUBLIC_API RendererConstPtr make_renderer_const_ptr(
    rcbe::rendering::renderer_config &&config,
    const std::shared_ptr<rcbe::rendering::RenderingContext> &context);
}// namespace rdmn::render

#endif//RDMN_ENGINE_RENDERER_HPP
