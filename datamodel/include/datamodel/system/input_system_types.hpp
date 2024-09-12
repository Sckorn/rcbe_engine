#ifndef RCBE_ENGINE_INPUT_SYSTEM_TYPES_HPP
#define RCBE_ENGINE_INPUT_SYSTEM_TYPES_HPP

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#ifdef __linux__
#include <rcbe-engine/datamodel/system/XKeyBoardEventType.hpp>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
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
    middle_button_press = WM_MBUTTONDOWN, // since not a bit flag might not work
    left_button_release = WM_LBUTTONUP,
    right_button_release = WM_RBUTTONUP,
    middle_button_release = WM_MBUTTONUP, // same as above
    mouse_motion = WM_MOUSEMOVE,
#endif

#endif
    unknown = -1
};

inline bool is_mouse_event(InputEventType t) {
#ifdef __linux__
    return (t == InputEventType::button_press || t == InputEventType::button_release || t == InputEventType::mouse_motion);
#else
#ifdef _WIN32
    return (t == InputEventType::left_button_press || t == InputEventType::left_button_release 
            || t == InputEventType::right_button_press || t == InputEventType::right_button_release
            || t == InputEventType::middle_button_press || t == InputEventType::middle_button_release);
#endif
#endif
}

inline bool is_keyboard_event(InputEventType t) {
    return (t == InputEventType::key_press || t == InputEventType::key_release);
}

using InputEventTypeBimap = boost::bimap<std::string, InputEventType>;

static const InputEventTypeBimap iet_table = boost::assign::list_of<InputEventTypeBimap::relation>
    ("key_press", InputEventType::key_press)
    ("key_release", InputEventType::key_release)
#ifdef __linux__
    ("button_press", InputEventType::button_press)
    ("button_release", InputEventType::button_release)
#endif
#ifdef _WIN32
    ("left_button_press", InputEventType::left_button_press)
    ("left_button_release", InputEventType::left_button_release)
    ("right_button_press", InputEventType::right_button_press)
    ("right_button_release", InputEventType::right_button_release)
    ("middle_button_press", InputEventType::middle_button_press)
    ("middle_button_release", InputEventType::middle_button_release)
#endif
    ("mouse_motion", InputEventType::mouse_motion);

static std::string str_from_input_event_type(InputEventType type) {
    auto it = iet_table.right.find(type);
    if (it != iet_table.right.end()) {
        return it->second;
    }
    return "unknown";
}

static InputEventType input_event_type_from_string(const std::string &type) {
    auto it = iet_table.left.find(type);
    if (it != iet_table.left.end()) {
        return it->second;
    }

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
    middle_button_press = WM_MBUTTONDOWN, // since not a bit flag might not work
    left_button_release = WM_LBUTTONUP,
    right_button_release = WM_RBUTTONUP,
    middle_button_release = WM_MBUTTONUP, // same as above
    wheel_up = WM_MOUSEWHEEL,
    wheel_down = WM_MOUSEWHEEL,
#endif

#endif
    unknown = -1
};

using MouseEventTypeBimap = boost::bimap<std::string, MouseEventType>;

static const MouseEventTypeBimap met_table = boost::assign::list_of<MouseEventTypeBimap::relation>
#ifdef __linux__
    ("left_button", MouseEventType::left_button)
    ("right_button", MouseEventType::right_button)
    ("middle_button", MouseEventType::middle_button)
#endif
#ifdef _WIN32
    ("left_button_press", MouseEventType::left_button_press)
    ("right_button_press", MouseEventType::right_button_press)
    ("middle_button_press", MouseEventType::middle_button_press)
    ("left_button_release", MouseEventType::left_button_release)
    ("right_button_release", MouseEventType::right_button_release)
    ("middle_button_release", MouseEventType::middle_button_release)
#endif
    ("wheel_up", MouseEventType::wheel_up)
    ("wheel_down", MouseEventType::wheel_down);

static std::string str_from_mouse_event_type(MouseEventType type) {
    auto it = met_table.right.find(type);
    if (it != met_table.right.end()) {
        return it->second;
    }
    return "uknown";
}

static MouseEventType mouse_event_type_from_string(const std::string &type) {
    auto it = met_table.left.find(type);
    if (it != met_table.left.end()) {
        return it->second;
    }

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

using InputValueTypeBimap = boost::bimap<std::string, InputValueType>;

static const InputValueTypeBimap ivttable = boost::assign::list_of<InputValueTypeBimap::relation>("switch", InputValueType::boolswitch)("axis", InputValueType::axis);

static std::string str_from_input_value_type(InputValueType type) {
    auto it = ivttable.right.find(type);
    if (it != ivttable.right.end()) {
        return it->second;
    }
    return "uknown";
}

static InputValueType input_value_type_from_string(const std::string &type) {
    auto it = ivttable.left.find(type);
    if (it != ivttable.left.end()) {
        return it->second;
    }

    return InputValueType ::unknown;
}

struct input_event_type_hash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

/*enum class KeyboardEventType {
    symbol_w = XK_w,         // XK_w
    symbol_W = XK_W,         // XK_W
    symbol_a = XK_a,         // XK_a
    symbol_A = XK_A,         // XK_A
    symbol_s = XK_s,         // XK_s
    symbol_S = XK_S,         // XK_S
    symbol_d = XK_d,         // XK_d
    symbol_D = XK_D,         // XK_D
    symbol_o = XK_o,         // XK_o
    symbol_O = XK_O,         // XK_O
    arrow_up = XK_Up,        // XK_uparrow
    arrow_left = XK_Left,    // XK_leftarrow
    arrow_right = XK_Right,  // XK_rightarrow
    arrow_down = XK_Down,    // XK_downarrow
    escape = XK_Escape,      // XK_Escape
    backspace = XK_BackSpace,// XK_BackSpace
    tab = XK_Tab,            // XK_Tab
    enter = XK_Return,       // XK_Return
    space = XK_space,        // XK_space
    tilde = XK_asciitilde    // XK_asciitilde
};*/

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
