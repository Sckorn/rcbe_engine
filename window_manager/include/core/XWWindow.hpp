#ifndef RCBE_ENGINE_XWWINDOW_HPP
#define RCBE_ENGINE_XWWINDOW_HPP

#include <memory>
#include <future>

#include <data_types/system/WindowConfig.hpp>
#include <data_types/rendering/RenderingContext.hpp>

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

    // TODO: add WindowContext class, to hide details about rott window and stuff
    XWWindow(WindowConfig &&config_, Display* root_display, int screen_number, const Window& root_window, const Atom& delete_msg);

    ~XWWindow();

    [[nodiscard]] const WindowConfig& get_config() const;

    decltype(auto) start_window_loop_aync() {
        return std::async(std::launch::async, [this]() {
            start_window_loop();
        });
    }
    void start_window_loop();
    void stop_window_loop();

    void kill();
    void show();
    [[nodiscard]]const std::shared_ptr<core::AbstractInputManager>& get_input_manager() const;
    void set_input_manager(const std::shared_ptr<AbstractInputManager>& manager);

    void on_configure(window::configure_handler_t&& handler);
    void on_unmap(window::unmap_handler_t&& handler);

    [[nodiscard]]const std::shared_ptr<rendering::RenderingContext>& get_context() const; // TODO: rename to getRenderingContext

    void set_renderer(rcbe::rendering::GLRendererPtr renderer_ptr);
    [[nodiscard]]const rcbe::rendering::GLRendererPtr& get_renderer() const;

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
