#ifndef RCBE_ENGINE_XWINDOW_HPP
#define RCBE_ENGINE_XWINDOW_HPP

#include <memory>
#include <mutex>
#include <atomic>
#include <type_traits>
#include <future>
#include <optional>

#include <rcbe-engine/datamodel/system/window_config.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>

#include <renderer/GLRenderer.hpp>

#include <core/InputManagerImplementation.hpp>
#include <core/AbstractInputManager.hpp>

namespace rcbe::core {
class XWindow {
public:

    XWindow(window_config &&config_, Display* root_display, int screen_number, const Window& root_window, const Atom& delete_msg);
    ~XWindow();

    [[nodiscard]] const window_config& get_config() const;


    decltype(auto) start_window_loop_aync() {
        return std::async(std::launch::async, [this]() {
            start_window_loop();
        });
    }
    void start_window_loop();
    void stop_window_loop();

    void on_configure(window::ConfigureHandlerType&& handler);
    void on_unmap(window::UunmapHandlerType&& handler);

    [[nodiscard]]const std::shared_ptr<rendering::RenderingContext>& get_context() const; // TODO: rename to getRenderingContext

    void kill();
    void map_window();
    void set_renderer(rendering::GLRendererPtr renderer_ptr);
    void set_manager(const std::shared_ptr<AbstractInputManager>& manager);

    [[nodiscard]] const rendering::GLRendererPtr& get_renderer() const;
    [[nodiscard]] const std::shared_ptr<AbstractInputManager>& get_input_manager() const;

private:

    void window_loop();

    window_config config_;
    std::mutex running_mutex_;
    std::mutex input_event_mutex_;
    mutable std::mutex renderer_access_mutex_;
    mutable std::mutex input_manager_access_mutex_;
    bool running_ = false;
    Display* root_display_ = nullptr;
    XSetWindowAttributes attributes_;
    std::shared_ptr<rendering::RenderingContext> rendering_context_ = nullptr;
    rendering::GLRendererPtr renderer_ = nullptr;
    std::shared_ptr<core::AbstractInputManager> input_manager_ = nullptr;

    std::mutex kill_mutex_;
    bool killed_ = false;

    window::UunmapHandlerType unmap_handler_;
    window::ConfigureHandlerType configure_handler_;
};
}

#endif //RCBE_ENGINE_XWINDOW_HPP
