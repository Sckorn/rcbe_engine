#ifndef RCBE_ENGINE_WINDOWCONTEXT_HPP
#define RCBE_ENGINE_WINDOWCONTEXT_HPP

#include <memory>
#include <mutex>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

namespace rcbe::core {
class R_PUBLIC_API WindowContext {
public:

    WindowContext() = default;
    ~WindowContext() = default;

#ifdef __linux__
    void setRootDisplay(Display *d);
    [[nodiscard]] Display *getRootDisplay() const;

    void setScreenNumber(int sn);
    [[nodiscard]] int getScreenNumber() const;

    void setRootWindow(Window w);
    [[nodiscard]] Window getRootWindow() const;

    void setDeleteMsg(Atom dmsg);
    [[nodiscard]] Atom getDeleteMsg() const;


private:

    mutable std::mutex display_mutex_;
    mutable std::mutex screen_num_mutex_;
    mutable std::mutex root_window_mutex_;
    mutable std::mutex delete_msg_mutex_;

    Display *root_display_ = nullptr;
    int screen_number_;
    // all ofthe below is actually unsigned long, so copying it
    Window root_window_;
    Atom delete_msg_;
#endif

#ifdef _WIN32
    [[nodiscard]] HINSTANCE getInstanceHandle() const;
    void setInstanceHandle(HINSTANCE hinst);

    [[nodiscard]] std::wstring getWindowClass() const;
    void setWindowClass(std::wstring &&window_class);

    [[nodiscard]] std::wstring getCommandLineParameters() const;
    void setCommandLineParameters(std::wstring &&cmd_params);

    [[nodiscard]] int getShowCommand() const;
    void setShowCommand(int command);

private:

    mutable std::mutex instance_handle_mutex_;
    mutable std::mutex window_class_mutex_;
    mutable std::mutex command_line_str_mutex_;
    mutable std::mutex window_show_command_mutex_;

    // Global Variables:
    HINSTANCE hinst_;                                // current instance
    /// TODO: @sckorn @radameon handle wstrings via build configurations
    std::wstring window_class_;            // the main window class name
    std::wstring command_line_params_str_;
    int window_show_command_code_;
#endif // _WIN32
};

using WindowContextPtr = std::shared_ptr<WindowContext>;
using WindowContextConstPtr = std::shared_ptr<const WindowContext>;
}// namespace rcbe::core

#endif//RCBE_ENGINE_WINDOWCONTEXT_HPP
