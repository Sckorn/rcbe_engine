#ifndef RCBE_ENGINE_WINDOWCONTEXT_HPP
#define RCBE_ENGINE_WINDOWCONTEXT_HPP

#include <memory>
#include <mutex>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

namespace rcbe::core {
class WindowContext {
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
#endif

private:

    mutable std::mutex display_mutex_;
    mutable std::mutex screen_num_mutex_;
    mutable std::mutex root_window_mutex_;
    mutable std::mutex delete_msg_mutex_;

#ifdef __linux__
    Display *root_display_ = nullptr;
    int screen_number_;
    // all ofthe below is actually unsigned long, so copying it
    Window root_window_;
    Atom delete_msg_;
#endif
};

using WindowContextPtr = std::shared_ptr<WindowContext>;
using WindowContextConstPtr = std::shared_ptr<const WindowContext>;
}// namespace rcbe::core

#endif//RCBE_ENGINE_WINDOWCONTEXT_HPP
