#include <rcbe-engine/core/WindowManager.hpp>
#include <rcbe-engine/core/XWWindow.hpp>

#include <nlohmann/json.hpp>

#include <boost/log/trivial.hpp>

static constexpr const char * x_del_window_msg = "WM_DELETE_WINDOW";

namespace rcbe::core {
WindowManager::WindowManager(bool multi_thread, size_t max_wins)
:
maximum_windows_ { max_wins }
, window_context_ { std::make_shared<WindowContext>() }
{
    if (multi_thread) {
        auto status = XInitThreads();
        if (!status)
            throw std::runtime_error("Can't init X threads");
    }

    window_context_->setRootDisplay(XOpenDisplay(nullptr));

    if (window_context_->getRootDisplay() == nullptr)
        throw std::runtime_error("Can't connect to X server");

    window_context_->setScreenNumber(XDefaultScreen(window_context_->getRootDisplay()));
    window_context_->setDeleteMsg(XInternAtom(window_context_->getRootDisplay(), x_del_window_msg, true));
    window_context_->setRootWindow(XRootWindow(window_context_->getRootDisplay(), window_context_->getScreenNumber()));
}

WindowPtr WindowManager::createWindow(window_config &&config) {
    if (windows_created_.size() == maximum_windows_) {
        throw std::runtime_error("Already maximum amount of windows!");
    }

    windows_created_.push(std::make_shared<XWWindow>(std::move(config), window_context_));
    return windows_created_.top();
}

WindowPtr WindowManager::createWindow(const nlohmann::json &json_config) {
    window_config c = json_config.at("window_config");
    return createWindow(std::move(c));
}

WindowManager::~WindowManager() {
    while(!windows_created_.empty()) {
        auto& w = windows_created_.top();
        w->kill();
        windows_created_.pop();
    }

    XCloseDisplay(window_context_->getRootDisplay());
}
}
