#ifndef RCBE_ENGINE_INPUT_MANAGER_HPP
#define RCBE_ENGINE_INPUT_MANAGER_HPP

#include <optional>
#include <stack>

#ifdef __linux__

#include <X11/X.h>
#include <X11/Xlib.h>

#endif 

#ifdef _WIN32
#include <Windows.h>
#include <rcbe-engine/core/windows_input_event.hpp>
#endif

namespace rcbe::utility {
class R_PUBLIC_API InputManagerTraits {
public:

    static constexpr size_t DEFAULT_MAXIMUM_DELEGATE_SIZE = 10;
    using KeyCodeType = unsigned long;
#ifdef __linux__
    using ManagerInputEventType = XEvent;
    using PreviousEventType = std::optional<XEvent>;
#endif
#ifdef _WIN32
    using ManagerInputEventType = rdmn::core::windows_input_event;
    using PreviousEventType = std::optional<rdmn::core::windows_input_event>;
#endif
    using InputEventReference = ManagerInputEventType &;
    using PreviousEventReference = PreviousEventType &;
    using InputEventConstReference = const ManagerInputEventType &;
    using PreviousEventConstReference = const PreviousEventType &;
};
}// namespace rcbe::utility

#endif//RCBE_ENGINE_INPUT_MANAGER_HPP
