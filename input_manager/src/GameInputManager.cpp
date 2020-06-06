#include <core/GameInputManager.hpp>

#include <common/utils/stack_utils.hpp>

#include <nlohmann/json.hpp>

namespace rcbe::core {
GameInputManager::GameInputManager(nlohmann::json&& j)
:
scheme_(std::move(j))
{

}

bool GameInputManager::try_set(const int keycode, const int value) {
    try {
        scheme_.set(keycode, value);
    } catch (const std::exception& ex) {
        BOOST_LOG_TRIVIAL(debug) << "Exception in handler: " << ex.what();
        return false;
    }

    return true;
}

void GameInputManager::init() {
    register_handler(InputEventType::key_press, [this](event_stack_type& events_stack) {
        auto e = utils::pop(events_stack);
        return try_set(e.xkey.keycode, 1);
    });
    register_handler(InputEventType::key_release, [this](event_stack_type& events_stack) {
        auto e = utils::pop(events_stack);
        return try_set(e.xkey.keycode, 0);
    });
    register_handler(InputEventType::button_press, [this](event_stack_type& events_stack) {
        auto e = utils::pop(events_stack);
        return try_set(e.xbutton.button, 1);
    });
    register_handler(InputEventType::button_release, [this](event_stack_type& events_stack) {
        auto e = utils::pop(events_stack);
        return try_set(e.xbutton.button, 0);
    });
    methods_initialized_ = true;
}

bool GameInputManager::try_process_event(input_event_reference event) {
    if (!methods_initialized_) {
        init();
    }

    return InputManagerImplementation::try_process_event(event);
}
}