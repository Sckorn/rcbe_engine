#ifndef RDMN_DATAMODEL_SYSTEM_WINDOWS_KEYBOARD_EVENT_TYPE_HPP
#define RDMN_DATAMODEL_SYSTEM_WINDOWS_KEYBOARD_EVENT_TYPE_HPP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>

namespace rdmn::core {
enum class WindowsKeyboardEventType {
    symbol_w = 0x57,         
    symbol_W = 0x57,         
    symbol_a = 0x41,         
    symbol_A = 0x41,         
    symbol_s = 0x53,         
    symbol_S = 0x53,         
    symbol_d = 0x44,         
    symbol_D = 0x44,         
    symbol_o = 0x4F,         
    symbol_O = 0x4F,         
    arrow_up = VK_UP,        
    arrow_left = VK_LEFT,    
    arrow_right = VK_RIGHT,  
    arrow_down = VK_DOWN,    
    escape = VK_ESCAPE,      
    backspace = VK_BACK,
    tab = VK_TAB,       
    enter = VK_RETURN,  
    space = VK_SPACE,   
    tilde = VK_OEM_3 
};
}

#endif // RDMN_DATAMODEL_SYSTEM_WINDOWS_KEYBOARD_EVENT_TYPE_HPP