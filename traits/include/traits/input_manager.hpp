#ifndef RCBE_ENGINE_INPUT_MANAGER_HPP
#define RCBE_ENGINE_INPUT_MANAGER_HPP

#include <stack>
#include <optional>

#include <X11/X.h>
#include <X11/Xlib.h>

namespace rcbe::utility {
class InputManagerTraits {
public:
    static constexpr size_t DEFAULT_MAXIMUM_DELEGATE_SIZE = 10;
    using key_code_type = unsigned long;
    using input_event_type = XEvent;
    using previous_event_type = std::optional<XEvent>;
    using input_event_reference = input_event_type&;
    using previous_event_reference = previous_event_type&;
    using input_event_const_reference = const input_event_type&;
    using previous_event_const_reference = const previous_event_type&;
};
}

#endif //RCBE_ENGINE_INPUT_MANAGER_HPP
