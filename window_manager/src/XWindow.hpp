#ifndef RCBE_ENGINE_XWINDOW_HPP
#define RCBE_ENGINE_XWINDOW_HPP

#include <memory>
#include <mutex>
#include <atomic>
#include <type_traits>
#include <future>

#include <data_types/system/WindowConfig.hpp>
#include <data_types/rendering/RenderingContext.hpp>

#include <renderer/GLRenderer.hpp>

namespace rcbe::core {
class XWindow {
public:

    XWindow(WindowConfig &&config_, Display* root_display, int screen_number, const Window& root_window, const Atom& delete_msg);
    ~XWindow();

    [[nodiscard]] const WindowConfig& get_config() const;


    decltype(auto) start_window_loop_aync() {
        return std::async(std::launch::async, [this]() {
            start_window_loop();
        });
    }
    void start_window_loop();
    void stop_window_loop();

    void on_configure(window::configure_handler_t&& handler);
    void on_unmap(window::unmap_handler_t&& handler);

    [[nodiscard]]const std::shared_ptr<rendering::RenderingContext>& get_context() const;

    void kill();
    void map_window();

    void set_renderer(rendering::GLRendererPtr renderer_ptr);

    [[nodiscard]] const rendering::GLRendererPtr& get_renderer() const;

private:

    void window_loop();

    WindowConfig config_;
    std::mutex running_mutex_;
    bool running_ = false;
    Display* root_display_ = nullptr;
    XSetWindowAttributes attributes_;
    std::shared_ptr<rendering::RenderingContext> rendering_context_ = nullptr;
    rendering::GLRendererPtr renderer_ = nullptr;

    std::mutex kill_mutex_;
    bool killed_ = false;

    window::unmap_handler_t unmap_handler_;
    window::configure_handler_t configure_handler_;
};
}

#endif //RCBE_ENGINE_XWINDOW_HPP
