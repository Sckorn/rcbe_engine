#include <rcbe-engine/datamodel/system/WindowContext.hpp>

namespace rcbe::core {
#ifdef __linux__
void WindowContext::setRootDisplay(Display *d) {
    std::lock_guard lg {display_mutex_};
    root_display_ = d;
}

Display *WindowContext::getRootDisplay() const {
    std::lock_guard lg {display_mutex_};
    return root_display_;
}

void WindowContext::setScreenNumber(int sn) {
    std::lock_guard lg {screen_num_mutex_};
    screen_number_ = sn;
}

int WindowContext::getScreenNumber() const {
    std::lock_guard lg {screen_num_mutex_};
    return screen_number_;
}

void WindowContext::setRootWindow(Window w) {
    std::lock_guard lg {root_window_mutex_};
    root_window_ = w;
}

Window WindowContext::getRootWindow() const {
    std::lock_guard lg {root_window_mutex_};
    return root_window_;
}

void WindowContext::setDeleteMsg(Atom dmsg) {
    std::lock_guard lg {delete_msg_mutex_};
    delete_msg_ = dmsg;
}

Atom WindowContext::getDeleteMsg() const {
    std::lock_guard lg {delete_msg_mutex_};
    return delete_msg_;
}
#endif

#ifdef _WIN32
HINSTANCE WindowContext::getInstanceHandle() const {
	std::lock_guard lg{instance_handle_mutex_};
	return hinst_;
}

void WindowContext::setInstanceHandle(HINSTANCE hinst) {
	std::lock_guard lg{instance_handle_mutex_};
	hinst_ = hinst;
}

std::wstring WindowContext::getWindowClass() const {
	std::lock_guard lg{window_class_mutex_};
	return window_class_;
}

void WindowContext::setWindowClass(std::wstring&& window_class) {
	std::lock_guard lg{window_class_mutex_};
	window_class_ = window_class;
}

std::wstring WindowContext::getCommandLineParameters() const {
	std::lock_guard lg{command_line_str_mutex_};
	return command_line_params_str_;
}

void WindowContext::setCommandLineParameters(std::wstring&& cmd_params) {
	std::lock_guard lg{command_line_str_mutex_};
	command_line_params_str_ = cmd_params;
}

int WindowContext::getShowCommand() const {
	std::lock_guard lg{window_show_command_mutex_};
	return window_show_command_code_;
}

void WindowContext::setShowCommand(int command) {
	std::lock_guard lg{window_show_command_mutex_};
	window_show_command_code_ = command;
}
#endif
}// namespace rcbe::core
