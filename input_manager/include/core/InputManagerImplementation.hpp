#ifndef RCBE_ENGINE_INPUTMANAGERIMPLEMENTATION_HPP
#define RCBE_ENGINE_INPUTMANAGERIMPLEMENTATION_HPP

#include <future>
#include <mutex>
#include <unordered_map>
#include <stack>

#include <boost/log/trivial.hpp>


#include <X11/X.h>
#include <X11/Xlib.h>

#include <common/delegate/AbstractDelegate.hpp>

#include <data_types/system/InputSystemTypes.hpp>
#include <traits/input_manager.hpp>

namespace rcbe::core {

static constexpr size_t DEFAULT_MAXIMUM_DELEGATE_SIZE = 10;

// shared from this, pass to window
// TODO: this should be renamed into InputManagerImpl, and relocated into private header
class InputManagerImplementation : public utility::InputManagerTraits {
public:

   using delegate_type = Delegate<void, handler_signature>;

   InputManagerImplementation() = default;
    ~InputManagerImplementation() = default;

    bool try_process_event(XEvent& event);

    template <typename HandlerType>
    void register_handler(InputEventType etype, HandlerType&& h) {
        static_assert(std::is_rvalue_reference_v<decltype(h)>, "HandlerType should be an rvalue reference!");
        auto event_type = static_cast<int>(etype);
        if (handlers_.find(etype) == handlers_.end()) {
            delegate_type d { DEFAULT_MAXIMUM_DELEGATE_SIZE };
            d += std::forward<HandlerType>(h);
            auto ret = handlers_.insert( {etype, core::AbstractDelegate { std::move(d) } } );
            if (!ret.second)
                throw std::runtime_error("Can't insert handler for " + std::to_string(event_type));
            else
                BOOST_LOG_TRIVIAL(debug) << "Inserted handler fot event type " << event_type;
        } else {
            handlers_.at(etype).as<handler_signature>() += std::move(h);
        }

        if (active_events_.find(event_type) == active_events_.end()) {
            active_events_[event_type] = false;
        }
    }

    [[nodiscard]]bool event_active(InputEventType event_type) const;

private:

    InputManagerMode mode_ = InputManagerMode::simple;
    int number_devices_ = 0;

    std::unordered_map<InputEventType, rcbe::core::AbstractDelegate, InputEventTypeHash> handlers_;
    std::unordered_map<int, bool> active_events_ = {
            {static_cast<int>(InputEventType::button_press), false},
            {static_cast<int>(InputEventType::button_release), false},
            {static_cast<int>(InputEventType::key_press), false},
            {static_cast<int>(InputEventType::key_release), false},
            {static_cast<int>(InputEventType::mouse_motion), false}
    };
    const std::unordered_map<InputEventType, InputEventType, InputEventTypeHash> exclusive_events_ = {
            {InputEventType ::button_press, InputEventType ::button_release},
            {InputEventType ::button_release, InputEventType ::button_press},
            {InputEventType ::key_press, InputEventType ::key_release},
            {InputEventType ::key_release, InputEventType ::key_press}
    };
    event_stack_type propagated_events_;
};

using InputManagerPtr = std::unique_ptr<InputManagerImplementation>;
using InputManagerConstPtr = std::unique_ptr<const InputManagerImplementation>;
}

#endif //RCBE_ENGINE_INPUTMANAGERIMPLEMENTATION_HPP
