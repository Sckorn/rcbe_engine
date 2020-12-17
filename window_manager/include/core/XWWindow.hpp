#ifndef RCBE_ENGINE_XWWINDOW_HPP
#define RCBE_ENGINE_XWWINDOW_HPP

#include <memory>
#include <future>

#include <rcbe-engine/datamodel/system/window_config.hpp>
#include <rcbe-engine/datamodel/system/WindowContext.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>

#include <X11/Xlib.h>

namespace rcbe::rendering {
class GLRenderer;

using GLRendererPtr = std::unique_ptr<GLRenderer>;
}

namespace rcbe::core {
class AbstractInputManager;
}

namespace rcbe::core {
class InputManagerImplementation;

#ifdef __linux__
class XWindow;
class XWWindow
{
public:
    XWWindow() = delete;

    XWWindow(window_config &&config_, const WindowContextPtr& window_context);

    ~XWWindow();

    [[nodiscard]] const window_config& getConfig() const;

    decltype(auto) startWindowLoopAync() {
        return std::async(std::launch::async, [this]() {
            startWindowLoop();
        });
    }
    void startWindowLoop();
    void stopWindowLoop();

    void kill();
    void show();
    [[nodiscard]]const std::shared_ptr<core::AbstractInputManager>& getInputManager() const;
    void setInputManager(const std::shared_ptr<AbstractInputManager>& manager);

    void onConfigure(window::ConfigureHandlerType&& handler);
    void onUnmap(window::UunmapHandlerType&& handler);

    [[nodiscard]]const std::shared_ptr<rendering::RenderingContext>& getRenderingContext() const;

    void setRenderer(rcbe::rendering::GLRendererPtr renderer_ptr);
    [[nodiscard]]const rcbe::rendering::GLRendererPtr& getRenderer() const;

private:
    std::shared_ptr<XWindow> impl_;
};

#elif  _WIN32
//TODO: implement it for windows
class XWWindow {

};


#endif

using WindowPtr = std::shared_ptr<XWWindow>;
using WindowConstPtr = std::shared_ptr<const XWWindow>;
}

#endif //RCBE_ENGINE_XWWINDOW_HPP
