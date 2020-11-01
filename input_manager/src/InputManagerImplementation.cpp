#include <chrono>

#include <boost/log/trivial.hpp>

#include <core/InputManagerImplementation.hpp>

namespace rcbe::core {

bool InputManagerImplementation::try_process_event(XEvent& event) {
    auto event_type = static_cast<InputEventType>(event.type);
    auto it = handlers_.find(event_type);
    if (it != handlers_.end()) {
        active_events_[event.type] = true;
        if (is_mouse_event(event_type)) {
            const auto button = static_cast<MouseEventType>(event.xbutton.button);
            mouse_buttons_states_.insert_or_assign(button, true);
        } else if (is_keyboard_event(event_type)) {
            const auto key_sym = XLookupKeysym(&(event.xkey), 0);
            const auto key = static_cast<KeyboardEventType>(key_sym);
            keyboard_buttons_states_.insert_or_assign(key, true);
        }

        auto itt = exclusive_events_.find(event_type);
        if (itt != exclusive_events_.end()) {
            active_events_[static_cast<int>(itt->second)] = false;

            if (itt->second == InputEventType::button_press) {
                disable_all_mouse();
            } else if (itt->second == InputEventType::key_press) {
                disable_all_keyboard();
            }
        }

        try {
            it->second.invoke(this->shared_from_this(), event, previous_event_);
        } catch (const std::exception& e) {
            previous_event_ = event;
            BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what();
            return false;
        }

        previous_event_ = event;

        return true;
    } else {
        BOOST_LOG_TRIVIAL(warning) << "Can't find handler for event type " << event.type;
        return false;
    }
}

bool InputManagerImplementation::event_active(InputEventType event_type) const {
    auto intetype = static_cast<int>(event_type);
    if (active_events_.find(intetype) != active_events_.end())
    {
        return active_events_.at(intetype);
    }
    return false;
}

bool InputManagerImplementation::get_value(MouseEventType type) const {
    auto it = mouse_buttons_states_.find(type);
    if (it != mouse_buttons_states_.end()) {
        return it->second;
    }

    return false;
}

bool InputManagerImplementation::get_value(KeyboardEventType type) const {
    auto it = keyboard_buttons_states_.find(type);
    if (it != keyboard_buttons_states_.end()) {
        return it->second;
    }

    return false;
}

void InputManagerImplementation::disable_all_mouse() {
    for (auto& i : mouse_buttons_states_) {
        i.second = false;
    }
}

void InputManagerImplementation::disable_all_keyboard() {
    for (auto& i : keyboard_buttons_states_) {
        i.second = false;
    }
}

}