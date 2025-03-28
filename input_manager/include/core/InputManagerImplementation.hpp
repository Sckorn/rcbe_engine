#ifndef RCBE_ENGINE_INPUTMANAGERIMPLEMENTATION_HPP
#define RCBE_ENGINE_INPUTMANAGERIMPLEMENTATION_HPP

#include <future>
#include <mutex>
#include <unordered_map>

#ifdef __linux__

#include <X11/X.h>
#include <X11/Xlib.h>

#endif

#include <rcbe-engine/datamodel/system/input_system_types.hpp>
#include <rcbe-engine/delegate/AbstractDelegate.hpp>
#include <rcbe-engine/core/InputManagerTraits.hpp>
#include <rdmn-engine/logger/trivial_logger.hpp>

namespace rcbe::core {

class R_PUBLIC_API InputManagerImplementation : public utility::InputManagerTraits {
public:

    using DelegateType = Delegate<void, InputManagerImplementation &, InputEventReference, PreviousEventReference>;
    using InvocationType = typename DelegateType::InvocationType;
    using HandlerIntermidiateStorage = std::pair<InputEventType, InvocationType>;

    virtual ~InputManagerImplementation() = default;

    bool tryProcessEvent(InputEventReference event);

    template <typename HandlerType>
    void registerHandler(InputEventType etype, HandlerType &&h) {
        static_assert(std::is_rvalue_reference_v<decltype(h)>, "HandlerType should be an rvalue reference!");
        auto event_type = static_cast<int>(etype);
        if (handlers_.find(etype) == handlers_.end()) {
            DelegateType d {DEFAULT_MAXIMUM_DELEGATE_SIZE};
            d += std::forward<HandlerType>(h);
            auto ret = handlers_.insert({etype, core::AbstractDelegate {std::move(d)}});
            if (!ret.second)
                throw std::runtime_error("Can't insert handler for " + std::to_string(event_type));
            else
                RDMN_LOG(RDMN_LOG_DEBUG) << "Inserted handler fot event type " << event_type;
        } else {
            handlers_.at(etype).as<InputManagerImplementation &, InputEventReference, PreviousEventReference>() += std::move(h);
        }

        if (active_events_.find(event_type) == active_events_.end()) {
            active_events_[event_type] = false;
        }
    }

    template <typename Handlers>
    void registerHandlers(std::vector<Handlers> &&h) {
        for (auto &&[type, handler] : h) {
            registerHandler(type, std::move(handler));
        }
    }

    [[nodiscard]] bool eventActive(InputEventType event_type) const;

    [[nodiscard]] bool getValue(MouseEventType type) const;
    [[nodiscard]] bool getValue(KeyboardEventType type) const;

protected:

    InputManagerImplementation() = default;

private:

    void disableAllMouse();
    void disableAllKeyboard();

    void excludeEvent(const InputEventType event_type_raw);

    InputManagerMode mode_ = InputManagerMode::simple;
    int number_devices_ = 0;

    std::unordered_map<InputEventType, rcbe::core::AbstractDelegate, input_event_type_hash> handlers_;
    std::unordered_map<int, bool> active_events_ = {
#ifdef __linux__
        {static_cast<int>(InputEventType::button_press), false},
        {static_cast<int>(InputEventType::button_release), false},
#endif
#ifdef _WIN32
        {static_cast<int>(InputEventType::left_button_press), false},
        {static_cast<int>(InputEventType::left_button_release), false},
        {static_cast<int>(InputEventType::right_button_press), false},
        {static_cast<int>(InputEventType::right_button_release), false},
        {static_cast<int>(InputEventType::middle_button_press), false},
        {static_cast<int>(InputEventType::middle_button_release), false},
#endif
        {static_cast<int>(InputEventType::key_press), false},
        {static_cast<int>(InputEventType::key_release), false},
        {static_cast<int>(InputEventType::mouse_motion), false}};
    const std::unordered_map<InputEventType, InputEventType, input_event_type_hash> exclusive_events_ = {
#ifdef __linux__
        {InputEventType ::button_press, InputEventType ::button_release},
        {InputEventType ::button_release, InputEventType ::button_press},
#endif
#ifdef _WIN32
        {InputEventType ::left_button_press, InputEventType ::left_button_release},
        {InputEventType ::left_button_release, InputEventType ::left_button_press},
        {InputEventType ::right_button_press, InputEventType ::right_button_release},
        {InputEventType ::right_button_release, InputEventType ::right_button_press},
        {InputEventType ::middle_button_press, InputEventType ::middle_button_release},
        {InputEventType ::middle_button_release, InputEventType ::middle_button_press},
#endif
        {InputEventType ::key_press, InputEventType ::key_release},
        {InputEventType ::key_release, InputEventType ::key_press}};

    std::unordered_map<MouseEventType, bool, input_mouse_button_hash> mouse_buttons_states_;
    std::unordered_map<KeyboardEventType, bool, input_keyboard_keys_hash> keyboard_buttons_states_;

    PreviousEventType previous_event_ = std::nullopt;
};

using InputManagerPtr = std::unique_ptr<InputManagerImplementation>;
using InputManagerConstPtr = std::unique_ptr<const InputManagerImplementation>;
}// namespace rcbe::core

#endif//RCBE_ENGINE_INPUTMANAGERIMPLEMENTATION_HPP
