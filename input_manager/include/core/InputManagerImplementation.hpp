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

// shared from this, pass to window
// TODO: this should be renamed into InputManagerImpl, and relocated into private header
class InputManagerImplementation : public utility::InputManagerTraits {
public:

    using delegate_type = Delegate<void, InputManagerImplementation&, input_event_reference, previous_event_reference>;
    using invocation_type = typename delegate_type::invocation_type;
    using handler_intermidiate_storage = std::pair<InputEventType, invocation_type>;

    virtual ~InputManagerImplementation() = default;

    bool try_process_event(input_event_reference event);

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
            handlers_.at(etype).as<InputManagerImplementation&, input_event_reference, previous_event_reference>() += std::move(h);
        }

        if (active_events_.find(event_type) == active_events_.end()) {
            active_events_[event_type] = false;
        }
    }

    template <typename Handlers>
    void register_handlers(std::vector<Handlers>&& h) {
        for (auto&& [type, handler] : h) {
            register_handler(type, std::move(handler));
        }
    }

    [[nodiscard]] bool event_active(InputEventType event_type) const;

    [[nodiscard]] bool get_value(MouseEventType type) const;
    [[nodiscard]] bool get_value(KeyboardEventType type) const;

protected:
    InputManagerImplementation() = default;

private:
    void disable_all_mouse();
    void disable_all_keyboard();

    void exclude_event(const InputEventType event_type_raw);

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

    std::unordered_map<MouseEventType, bool, InputMouseButtonHash> mouse_buttons_states_;
    std::unordered_map<KeyboardEventType, bool, InputKeyboardKeysHash> keyboard_buttons_states_;

    previous_event_type previous_event_ = std::nullopt;
};

using InputManagerPtr = std::unique_ptr<InputManagerImplementation>;
using InputManagerConstPtr = std::unique_ptr<const InputManagerImplementation>;
}

#endif //RCBE_ENGINE_INPUTMANAGERIMPLEMENTATION_HPP
