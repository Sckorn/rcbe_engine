#ifndef RCBE_ENGINE_WINDOWMANAGER_HPP
#define RCBE_ENGINE_WINDOWMANAGER_HPP

#include <memory>
#include <string>
#include <array>
#include <stack>

#include <X11/Xlib.h>

#include <data_types/system/WindowConfig.hpp>

#include <nlohmann/json_fwd.hpp>
#include <core/XWWindow.hpp>

namespace rcbe::core {

class WindowManager {
public:
    WindowManager() = delete;
    explicit WindowManager(bool multi_thread, size_t max_windows = 5);
    ~WindowManager();

    WindowPtr create_window(WindowConfig &&config);
    WindowPtr create_window(const nlohmann::json &json_config);

private:
    size_t maximum_windows_;

    Display* x_display_ = nullptr;
    int screen_number_;
    Window root_window_;
    Atom delete_message_;

    std::stack<WindowPtr> windows_created_;
};
}

#endif //RCBE_ENGINE_WINDOWMANAGER_HPP
