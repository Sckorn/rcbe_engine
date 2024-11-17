#ifndef RDMN_WINDOWS_INPUT_EVENT_HPP
#define RDMN_WINDOWS_INPUT_EVENT_HPP

#include <Windows.h>

namespace rdmn::core {
struct windows_input_event {
    UINT type;
    WPARAM wparam;
    LPARAM lparam;
};
}

#endif // RDMN_WINDOWS_INPUT_EVENT_HPP