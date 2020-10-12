#ifndef RCBE_ENGINE_INPUT_MANAGER_HPP
#define RCBE_ENGINE_INPUT_MANAGER_HPP

#include <stack>

#include <X11/X.h>
#include <X11/Xlib.h>

namespace rcbe::utility {
class InputManagerTraits {
public:
    using key_code_type = unsigned long;
    using input_event_type = XEvent;
    using input_event_reference = input_event_type&;
    using input_event_const_reference = const input_event_reference;
    using event_stack_type = std::stack<XEvent>;
    using handler_signature = event_stack_type &;
};
}

#endif //RCBE_ENGINE_INPUT_MANAGER_HPP
