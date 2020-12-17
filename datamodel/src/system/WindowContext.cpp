#include <rcbe-engine/datamodel/system/WindowContext.hpp>

namespace rcbe::core {
void WindowContext::setRootDisplay(Display * d) {
    std::lock_guard lg{display_mutex_};
    root_display_ = d;
}

Display* WindowContext::getRootDisplay() const {
    std::lock_guard lg{display_mutex_};
    return root_display_;
}

void WindowContext::setScreenNumber(int sn) {
    std::lock_guard lg{screen_num_mutex_};
    screen_number_ = sn;
}

int WindowContext::getScreenNumber() const {
    std::lock_guard lg{screen_num_mutex_};
    return screen_number_;
}

void WindowContext::setRootWindow(Window w) {
    std::lock_guard lg{root_window_mutex_};
    root_window_ = w;
}

Window WindowContext::getRootWindow() const {
    std::lock_guard lg{root_window_mutex_};
    return root_window_;
}

void WindowContext::setDeleteMsg(Atom dmsg) {
    std::lock_guard lg{delete_msg_mutex_};
    delete_msg_ = dmsg;
}

Atom WindowContext::getDeleteMsg() const {
    std::lock_guard lg{delete_msg_mutex_};
    return delete_msg_;
}
}
