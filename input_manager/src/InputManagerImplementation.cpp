#include <chrono>

#include <rcbe-engine/core/InputManagerImplementation.hpp>
#ifdef _WIN32
#include <rcbe-engine/core/windows_input_event.hpp>
#endif

namespace rcbe::core {

#ifdef __linux__
bool InputManagerImplementation::tryProcessEvent(XEvent &event) {
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

        excludeEvent(event_type);

        try {
            it->second.invoke(*this, event, previous_event_);
        } catch (const std::exception &e) {
            RDMN_LOG(RDMN_LOG_ERROR) << "Exception: " << e.what();
            previous_event_ = event;
            return false;
        }
        previous_event_ = event;

        return true;
    } else {
        RDMN_LOG(RDMN_LOG_WARN) << "Can't find handler for event type " << event.type;
        excludeEvent(event_type);
        return false;
    }
}
#endif
#ifdef _WIN32
bool InputManagerImplementation::tryProcessEvent(rdmn::core::windows_input_event &event) {
    auto event_type = static_cast<InputEventType>(event.type);
    auto it = handlers_.find(event_type);
    if (it != handlers_.end()) {
        active_events_[event.type] = true;
        if (is_mouse_event(event_type)) {
            UINT button_code = GET_XBUTTON_WPARAM(event.wparam);
            const auto button = static_cast<MouseEventType>(button_code);
            mouse_buttons_states_.insert_or_assign(button, true);
        } else if (is_keyboard_event(event_type)) {
            const auto key = static_cast<KeyboardEventType>(event.wparam);
            keyboard_buttons_states_.insert_or_assign(key, true);
        }

        excludeEvent(event_type);

        try {
            it->second.invoke(*this, event, previous_event_);
        } catch (const std::exception &e) {
            RDMN_LOG(RDMN_LOG_ERROR) << "Exception: " << e.what();
            previous_event_ = event;
            return false;
        }
        previous_event_ = event;

        return true;
    } else {
        RDMN_LOG(RDMN_LOG_WARN) << "Can't find handler for event type " << event.type;
        excludeEvent(event_type);
        return false;
    }
}
#endif

bool InputManagerImplementation::eventActive(InputEventType event_type) const {
    auto intetype = static_cast<int>(event_type);
    if (active_events_.find(intetype) != active_events_.end()) {
        return active_events_.at(intetype);
    }
    return false;
}

bool InputManagerImplementation::getValue(MouseEventType type) const {
    auto it = mouse_buttons_states_.find(type);
    if (it != mouse_buttons_states_.end()) {
        return it->second;
    }

    return false;
}

bool InputManagerImplementation::getValue(KeyboardEventType type) const {
    auto it = keyboard_buttons_states_.find(type);
    if (it != keyboard_buttons_states_.end()) {
        return it->second;
    }

    return false;
}

void InputManagerImplementation::disableAllMouse() {
    for (auto &i : mouse_buttons_states_) {
        i.second = false;
    }
}

void InputManagerImplementation::disableAllKeyboard() {
    for (auto &i : keyboard_buttons_states_) {
        i.second = false;
    }
}

void InputManagerImplementation::excludeEvent(const InputEventType event_type_raw) {
    auto itt = exclusive_events_.find(event_type_raw);

    if (itt != exclusive_events_.end()) {
        active_events_[static_cast<int>(itt->second)] = false;
#ifdef __linux__
        if (itt->second == InputEventType::button_press) {
#endif
#ifdef _WIN32
        bool mouse_pressed = (itt->second == InputEventType::left_button_press) 
            || (itt->second == InputEventType::right_button_press) 
            || (itt->second == InputEventType::middle_button_press);
        if (mouse_pressed) {
#endif
            disableAllMouse();
        } else if (itt->second == InputEventType::key_press) {
            disableAllKeyboard();
        }

    }
}
}// namespace rcbe::core