#ifndef RCBE_ENGINE_INPUT_MANAGER_HPP
#define RCBE_ENGINE_INPUT_MANAGER_HPP

#include <optional>
#include <stack>

#include <X11/X.h>
#include <X11/Xlib.h>

namespace rcbe::utility {
class InputManagerTraits {
public:

    static constexpr size_t DEFAULT_MAXIMUM_DELEGATE_SIZE = 10;
    using KeyCodeType = unsigned long;
    using ManagerInputEventType = XEvent;
    using PreviousEventType = std::optional<XEvent>;
    using InputEventReference = ManagerInputEventType &;
    using PreviousEventReference = PreviousEventType &;
    using InputEventConstReference = const ManagerInputEventType &;
    using PreviousEventConstReference = const PreviousEventType &;
};
}// namespace rcbe::utility

#endif//RCBE_ENGINE_INPUT_MANAGER_HPP
