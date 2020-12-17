#ifndef RCBE_ENGINE_WINDOWMANAGER_HPP
#define RCBE_ENGINE_WINDOWMANAGER_HPP

#include <memory>
#include <string>
#include <array>
#include <stack>

#include <X11/Xlib.h>

#include <rcbe-engine/datamodel/system/window_config.hpp>
#include <rcbe-engine/datamodel/system/WindowContext.hpp>

#include <nlohmann/json_fwd.hpp>
#include <rcbe-engine/core/XWWindow.hpp>

namespace rcbe::core {

class WindowManager {
public:
    WindowManager() = delete;
    explicit WindowManager(bool multi_thread, size_t max_windows = 5);
    ~WindowManager();

    WindowPtr createWindow(window_config &&config);
    WindowPtr createWindow(const nlohmann::json &json_config);

private:
    size_t maximum_windows_;

    WindowContextPtr window_context_;

    std::stack<WindowPtr> windows_created_;
};
}

#endif //RCBE_ENGINE_WINDOWMANAGER_HPP
