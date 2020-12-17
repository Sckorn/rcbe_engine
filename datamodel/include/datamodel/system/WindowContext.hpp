#ifndef RCBE_ENGINE_WINDOWCONTEXT_HPP
#define RCBE_ENGINE_WINDOWCONTEXT_HPP

#include <mutex>
#include <memory>

#include <X11/Xlib.h>

namespace rcbe::core {
class WindowContext {
public:
    WindowContext() = default;
    ~WindowContext() = default;

    void setRootDisplay(Display * d);
    [[nodiscard]] Display* getRootDisplay() const;

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

    Display* root_display_ = nullptr;
    int screen_number_;
    // all ofthe below is actually unsigned long, so copying it
    Window root_window_;
    Atom delete_msg_;
};

using WindowContextPtr = std::shared_ptr<WindowContext>;
using WindowContextConstPtr = std::shared_ptr<const WindowContext>;
}

#endif //RCBE_ENGINE_WINDOWCONTEXT_HPP
