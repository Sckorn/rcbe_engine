#ifndef RCBE_ENGINE_INPUT_SYSTEM_TYPES_HPP
#define RCBE_ENGINE_INPUT_SYSTEM_TYPES_HPP

#ifdef __linux__
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <rcbe-engine/datamodel/system/XKeyBoardEventType.hpp>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>

#include <rcbe-engine/datamodel/system/WindowsKeyBoardEventType.hpp>
#endif

namespace rcbe::core {

enum class InputManagerMode {
    simple,
    xextensions
};

enum class InputEventType {
#ifdef __linux__
    key_press = KeyPress,
    key_release = KeyRelease,
    button_press = ButtonPress,
    button_release = ButtonRelease,
    mouse_motion = MotionNotify,
#else

#ifdef _WIN32
    key_press = WM_KEYDOWN,
    key_release = WM_KEYUP,
    left_button_press = WM_LBUTTONDOWN,
    right_button_press = WM_RBUTTONDOWN,
    middle_button_press = WM_MBUTTONDOWN,// since not a bit flag might not work
    left_button_release = WM_LBUTTONUP,
    right_button_release = WM_RBUTTONUP,
    middle_button_release = WM_MBUTTONUP,// same as above
    mouse_motion = WM_MOUSEMOVE,
#endif

#endif
    unknown = -1
};

inline bool is_mouse_event(InputEventType t) {
#ifdef __linux__
    return (t == InputEventType::button_press || t == InputEventType::button_release || t == InputEventType::mouse_motion);
#endif
#ifdef _WIN32
    return (t == InputEventType::left_button_press || t == InputEventType::left_button_release || t == InputEventType::right_button_press || t == InputEventType::right_button_release || t == InputEventType::middle_button_press || t == InputEventType::middle_button_release);
#endif
}

inline bool is_keyboard_event(InputEventType t) {
    return (t == InputEventType::key_press || t == InputEventType::key_release);
}

static std::string str_from_input_event_type(InputEventType type) {
    switch (type) {
        case InputEventType::mouse_motion:
            return "mouse_motion";
        case InputEventType::key_press:
            return "key_press";
        case InputEventType::key_release:
            return "key_release";

#ifdef _WIN32
        case InputEventType::left_button_press:
            return "left_button_press";
        case InputEventType::left_button_release:
            return "left_button_release";
        case InputEventType::right_button_press:
            return "right_button_press";
        case InputEventType::right_button_release:
            return "right_button_release";
        case InputEventType::middle_button_press:
            return "middle_button_press";
        case InputEventType::middle_button_release:
            return "middle_button_release";
#endif

#ifdef __linux__
        case InputEventType::button_press:
            return "button_press";
        case InputEventType::button_release:
            return "button_release";
#endif
        default:
            return "unknown";
    }
}

static InputEventType input_event_type_from_string(const std::string &type) {
    if (type == "mouse_motion")
        return InputEventType::mouse_motion;
    if (type == "key_press")
        return InputEventType::key_press;
    if (type == "key_release")
        return InputEventType::key_release;

#ifdef _WIN32
    if (type == "left_button_press")
        return InputEventType::left_button_press;
    if (type == "left_button_release")
        return InputEventType::left_button_release;
    if (type == "right_button_press")
        return InputEventType::right_button_press;
    if (type == "right_button_release")
        return InputEventType::right_button_release;
    if (type == "middle_button_press")
        return InputEventType::middle_button_press;
    if (type == "middle_button_release")
        return InputEventType::middle_button_release;
#endif

#ifdef __linux__
    if (type == "button_press")
        return InputEventType::button_press;
    if (type == "button_release")
        return InputEventType::button_release;
#endif

    return InputEventType ::unknown;
}

enum class MouseEventType {
#ifdef __linux__
    left_button = Button1,
    right_button = Button3,
    middle_button = Button2,
    wheel_up = Button4,
    wheel_down = Button5,
#else

#ifdef _WIN32
    left_button_press = WM_LBUTTONDOWN,
    right_button_press = WM_RBUTTONDOWN,
    middle_button_press = WM_MBUTTONDOWN,// since not a bit flag might not work
    left_button_release = WM_LBUTTONUP,
    right_button_release = WM_RBUTTONUP,
    middle_button_release = WM_MBUTTONUP,// same as above
    wheel_up = WM_MOUSEWHEEL,
    wheel_down = WM_MOUSEWHEEL,
#endif

#endif
    unknown = -1
};

static std::string str_from_mouse_event_type(MouseEventType type) {
    switch (type) {
#ifdef __linux__
        case MouseEventType::wheel_up:
            return "wheel_up";
        case MouseEventType::wheel_down:
            return "wheel_down";
#endif

#ifdef _WIN32
        case MouseEventType::wheel_up:
            return "mousewheel";
        case MouseEventType::left_button_press:
            return "left_button_press";
        case MouseEventType::right_button_press:
            return "right_button_press";
        case MouseEventType::middle_button_press:
            return "middle_button_press";
        case MouseEventType::left_button_release:
            return "left_button_release";
        case MouseEventType::right_button_release:
            return "right_button_release";
        case MouseEventType::middle_button_release:
            return "middle_button_release";
#endif

#ifdef __linux__
        case MouseEventType::left_button:
            return "left_button";
        case MouseEventType::right_button:
            return "right_button";
        case MouseEventType::middle_button:
            return "middle_button";
#endif
        default:
            return "unknown";
    }
}

static MouseEventType mouse_event_type_from_string(const std::string &type) {
    if (type == "wheel_up")
        return MouseEventType::wheel_up;
    if (type == "wheel_down")
        return MouseEventType::wheel_down;

#ifdef _WIN32
    if (type == "left_button_press")
        return MouseEventType::left_button_press;
    if (type == "right_button_press")
        return MouseEventType::right_button_press;
    if (type == "middle_button_press")
        return MouseEventType::middle_button_press;
    if (type == "left_button_release")
        return MouseEventType::left_button_release;
    if (type == "right_button_release")
        return MouseEventType::right_button_release;
    if (type == "middle_button_release")
        return MouseEventType::middle_button_release;
#endif

#ifdef __linux__
    if (type == "left_button")
        return MouseEventType::left_button;
    if (type == "right_button")
        return MouseEventType::right_button;
    if (type == "middle_button")
        return MouseEventType::middle_button;
#endif

    return MouseEventType ::unknown;
}

struct input_mouse_button_hash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

enum class InputValueType {
    boolswitch,
    axis,
    unknown
};

static std::string str_from_input_value_type(InputValueType type) {
    switch (type) {
        case InputValueType::boolswitch:
            return "switch";
        case InputValueType::axis:
            return "axis";
        case InputValueType::unknown:
        default:
            return "uknown";
    }
}

static InputValueType input_value_type_from_string(const std::string &type) {
    if (type == "switch")
        return InputValueType::boolswitch;

    if (type == "axis")
        return InputValueType::axis;

    return InputValueType ::unknown;
}

struct input_event_type_hash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

#ifdef __linux__
using KeyboardEventType = rdmn::core::XKeyboardEventType;
#else

#ifdef _WIN32
using KeyboardEventType = rdmn::core::WindowsKeyboardEventType;
#endif

#endif

struct input_keyboard_keys_hash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

}// namespace rcbe::core

#endif//RCBE_ENGINE_INPUT_SYSTEM_TYPES_HPP
