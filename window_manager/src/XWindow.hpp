#ifndef RCBE_ENGINE_XWINDOW_HPP
#define RCBE_ENGINE_XWINDOW_HPP

#include <memory>
#include <mutex>
#include <atomic>
#include <type_traits>
#include <future>
#include <optional>

#include <rcbe-engine/datamodel/system/window_config.hpp>
#include <rcbe-engine/datamodel/system/WindowContext.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>

#include <rcbe-engine/renderer/GLRenderer.hpp>

#include <rcbe-engine/core/InputManagerImplementation.hpp>
#include <rcbe-engine/core/AbstractInputManager.hpp>

namespace rcbe::core {
class XWindow {
public:

    XWindow(window_config &&config_, const WindowContextPtr& window_context);
    ~XWindow();

    [[nodiscard]] const window_config& getConfig() const;


    decltype(auto) startWindowLoopAync() {
        return std::async(std::launch::async, [this]() {
            startWindowLoop();
        });
    }
    void startWindowLoop();
    void stopWindowLoop();

    void onConfigure(window::ConfigureHandlerType&& handler);
    void onUnmap(window::UunmapHandlerType&& handler);

    [[nodiscard]]const std::shared_ptr<rendering::RenderingContext>& getRenderingContext() const;

    void kill();
    void mapWindow();
    void setRenderer(rendering::GLRendererPtr renderer_ptr);
    void setInputManager(const std::shared_ptr<AbstractInputManager>& manager);

    [[nodiscard]] const rendering::GLRendererPtr& getRenderer() const;
    [[nodiscard]] const std::shared_ptr<AbstractInputManager>& getInputManager() const;

private:

    void windowLoop();

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
