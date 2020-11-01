#ifndef RCBE_ENGINE_INPUTSYSTEMTYPES_HPP
#define RCBE_ENGINE_INPUTSYSTEMTYPES_HPP

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>

namespace rcbe::core {

enum class InputManagerMode {
    simple,
    xextensions
};

enum class InputEventType {
    key_press = KeyPress,
    key_release = KeyRelease,
    button_press = ButtonPress,
    button_release = ButtonRelease,
    mouse_motion = MotionNotify,
    unknown = -1
};

using InputEventTypeBimap = boost::bimap<std::string, InputEventType>;

static const InputEventTypeBimap iet_table = boost::assign::list_of<InputEventTypeBimap::relation>
        ("key_press", InputEventType::key_press)
        ("key_release", InputEventType::key_release)
        ("button_press", InputEventType::button_press)
        ("button_release", InputEventType::button_release)
        ("mouse_motion", InputEventType::mouse_motion);

static std::string str_from_input_event_type(InputEventType type) {
    auto it = iet_table.right.find(type);
    if (it != iet_table.right.end())
    {
        return it->second;
    }
    return "unknown";
}

static InputEventType input_event_type_from_string(const std::string& type) {
    auto it = iet_table.left.find(type);
    if (it != iet_table.left.end())
    {
        return it->second;
    }

    return InputEventType ::unknown;
}

enum class MouseEventType {
    left_button = Button1,
    right_button = Button2,
    middle_button = Button3,
    wheel_up = Button4,
    wheel_down = Button5,
    unknown = -1
};

using MouseEventTypeBimap = boost::bimap<std::string, MouseEventType>;

static const MouseEventTypeBimap met_table = boost::assign::list_of<MouseEventTypeBimap::relation>
        ("left_button", MouseEventType::left_button)
        ("right_button", MouseEventType::right_button)
        ("middle_button", MouseEventType::middle_button)
        ("wheel_up", MouseEventType::wheel_up)
        ("wheel_down", MouseEventType::wheel_down);

static std::string str_from_mouse_event_type(MouseEventType type) {
    auto it = met_table.right.find(type);
    if (it != met_table.right.end())
    {
        return it->second;
    }
    return "uknown";
}

static MouseEventType mouse_event_type_from_string(const std::string& type) {
    auto it = met_table.left.find(type);
    if (it != met_table.left.end())
    {
        return it->second;
    }

    return MouseEventType ::unknown;
}

enum class InputValueType {
    boolswitch,
    axis,
    unknown
};

using InputValueTypeBimap = boost::bimap<std::string, InputValueType>;

static const InputValueTypeBimap ivttable = boost::assign::list_of<InputValueTypeBimap::relation>
        ("switch", InputValueType::boolswitch)
        ("axis", InputValueType::axis);

static std::string str_from_input_value_type(InputValueType type) {
    auto it = ivttable.right.find(type);
    if (it != ivttable.right.end())
    {
        return it->second;
    }
    return "uknown";
}

static InputValueType input_value_type_from_string(const std::string& type) {
    auto it = ivttable.left.find(type);
    if (it != ivttable.left.end())
    {
        return it->second;
    }

    return InputValueType ::unknown;
}

struct InputEventTypeHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

enum class KeyboardEventType {
    unknown = -1,
    symbol_w = 0x0077,      // XK_w
    symbol_W = 0x0057,      // XK_W
    symbol_a = 0x0061,      // XK_a
    symbol_A = 0x0041,      // XK_A
    symbol_s = 0x0073,      // XK_w
    symbol_S = 0x0053,      // XK_W
    symbol_d = 0x0064,      // XK_w
    symbol_D = 0x0044,      // XK_W
    arrow_up = 0x08fc,      // XK_uparrow
    arrow_left = 0x08fb,    // XK_leftarrow
    arrow_right = 0x08fd,   // XK_rightarrow
    arrow_down = 0x08fe,    // XK_downarrow
    escape = 0xff1b,        // XK_Escape
    backspace = 0xff08,     // XK_BackSpace
    tab = 0xff09,           // XK_Tab
    enter = 0xff0d,         // XK_Return
    space = 0x0020,         // XK_space
    end = 255
};

struct InputKeyboardKeysHash {
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};
}

#endif //RCBE_ENGINE_INPUTSYSTEMTYPES_HPP
