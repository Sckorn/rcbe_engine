#ifndef RCBE_ENGINE_WINDOWMANAGER_HPP
#define RCBE_ENGINE_WINDOWMANAGER_HPP

#include <array>
#include <memory>
#include <stack>
#include <string>

#include <nlohmann/json_fwd.hpp>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

#include <rcbe-engine/core/RWindow.hpp>
#include <rcbe-engine/datamodel/system/WindowContext.hpp>
#include <rcbe-engine/datamodel/system/window_config.hpp>

namespace rcbe::core {

class R_PUBLIC_API WindowManager {
public:

    WindowManager() = delete;
#ifdef __linux__
    WindowManager(bool multi_thread, size_t max_windows = 5);
#endif
#ifdef _WIN32
    WindowManager(HINSTANCE instance, std::wstring window_class, int cmd_show, size_t max_windows = 5);
#endif
    ~WindowManager();

    WindowPtr createWindow(window_config &&config);
    WindowPtr createWindow(const nlohmann::json &json_config);

private:

    size_t maximum_windows_;

    WindowContextPtr window_context_;

    std::stack<WindowPtr> windows_created_;
};
}// namespace rcbe::core

#endif//RCBE_ENGINE_WINDOWMANAGER_HPP
