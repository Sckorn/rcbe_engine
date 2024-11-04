#ifndef RDMN_DATAMODEL_SYSTEM_X_KEYBOARD_EVENT_TYPE_HPP
#define RDMN_DATAMODEL_SYSTEM_X_KEYBOARD_EVENT_TYPE_HPP

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace rdmn::core {
enum class XKeyboardEventType {
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
};
}

#endif // RDMN_DATAMODEL_SYSTEM_WINDOWS_KEYBOARD_EVENT_TYPE_HPP