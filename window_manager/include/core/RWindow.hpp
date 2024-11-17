#ifndef RCBE_ENGINE_XWWINDOW_HPP
#define RCBE_ENGINE_XWWINDOW_HPP

#include <future>
#include <memory>
#include <optional>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>
#include <rcbe-engine/datamodel/system/WindowContext.hpp>
#include <rcbe-engine/datamodel/system/window_config.hpp>

namespace rdmn::render {
class Renderer;

using RendererPtr = std::unique_ptr<Renderer>;
}// namespace rdmn::render

namespace rcbe::core {
class AbstractInputManager;
}

namespace rcbe::core {
class InputManagerImplementation;

#ifdef __linux__
class XWindow;
#elif defined(_WIN32)
class MsWindow;
#endif
class R_PUBLIC_API RWindow {
public:
#ifdef _WIN32
    using GraphicContext = HDC;
#endif
#ifdef __linux__
    using GraphicContext = GC;
#endif

    RWindow() = delete;

    RWindow(window_config &&config_, const WindowContextPtr &window_context);

    ~RWindow();

    [[nodiscard]] const window_config &getConfig() const;

    decltype(auto) startWindowLoopAsync() {
        return std::async(std::launch::async, [this]() {
            startWindowLoop();
        });
    }
    void startWindowLoop();
    void stopWindowLoop();

    void kill();
    void show();

    [[nodiscard]] const std::shared_ptr<core::AbstractInputManager> &getInputManager() const;
    void setInputManager(const std::shared_ptr<AbstractInputManager> &manager);

    void onConfigure(window::ConfigureHandlerType &&handler);
    void onUnmap(window::UunmapHandlerType &&handler);

    [[nodiscard]] const std::shared_ptr<rendering::RenderingContext> &getRenderingContext() const;

    [[nodiscard]] std::optional<GraphicContext> getGraphicContext() const;

#ifdef _WIN32
    [[nodiscard]] HWND getWindowHandle() const;
#endif

    void setRenderer(rdmn::render::RendererPtr renderer_ptr);
    [[nodiscard]] const rdmn::render::RendererPtr &getRenderer() const;

private:
#ifdef __linux__
    std::shared_ptr<XWindow> impl_;
#elif defined(_WIN32)
    std::shared_ptr<MsWindow> impl_;
#endif
};

using WindowPtr = std::shared_ptr<RWindow>;
using WindowConstPtr = std::shared_ptr<const RWindow>;
}// namespace rcbe::core

#endif//RCBE_ENGINE_XWWINDOW_HPP
