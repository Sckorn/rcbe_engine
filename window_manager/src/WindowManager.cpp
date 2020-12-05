#include <core/WindowManager.hpp>
#include <core/XWWindow.hpp>

#include <nlohmann/json.hpp>

#include <boost/log/trivial.hpp>

static constexpr const char * x_del_window_msg = "WM_DELETE_WINDOW";

namespace rcbe::core {
WindowManager::WindowManager(bool multi_thread, size_t max_wins)
:
maximum_windows_ { max_wins }
{
    if (multi_thread) {
        auto status = XInitThreads();
        if (!status)
            throw std::runtime_error("Can't init X threads");
    }

    x_display_ = XOpenDisplay(NULL);
    if (x_display_ == nullptr)
        throw std::runtime_error("Can't connect to X server");

    screen_number_ = XDefaultScreen(x_display_);
    delete_message_ = XInternAtom(x_display_, x_del_window_msg, true);
    root_window_ = XRootWindow(x_display_, screen_number_);
}

WindowPtr WindowManager::create_window(WindowConfig &&config) {
    if (windows_created_.size() == maximum_windows_) {
        throw std::runtime_error("Already maximum amount of windows!");
    }

    windows_created_.push(std::make_shared<XWWindow>(std::move(config), x_display_, screen_number_, root_window_, delete_message_));
    return windows_created_.top();
}

WindowPtr WindowManager::create_window(const nlohmann::json &json_config) {
    WindowConfig c = json_config.at("window_config");
    return create_window(std::move(c));
}

// TODO: iterate over created windows and kill them all
WindowManager::~WindowManager() {
    XCloseDisplay(x_display_);
}
}
