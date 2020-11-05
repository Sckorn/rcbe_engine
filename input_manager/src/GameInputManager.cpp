#include <core/GameInputManager.hpp>

#include <common/utils/stack_utils.hpp>

#include <nlohmann/json.hpp>

namespace rcbe::core {
GameInputManager::GameInputManager(nlohmann::json&& j)
:
scheme_(std::move(j))
{
    register_handler(InputEventType::key_press, [](InputManagerImplementationPtr im, input_event_reference event, previous_event_reference previous) {
        auto actual = std::static_pointer_cast<GameInputManager>(im);
        return actual->try_set(event.xkey.keycode, 1);
    });
    register_handler(InputEventType::key_release, [](InputManagerImplementationPtr im, input_event_reference event, previous_event_reference previous) {
        auto actual = std::static_pointer_cast<GameInputManager>(im);
        return actual->try_set(event.xkey.keycode, 0);
    });
    register_handler(InputEventType::button_press, [](InputManagerImplementationPtr im, input_event_reference event, previous_event_reference previous) {
        auto actual = std::static_pointer_cast<GameInputManager>(im);
        return actual->try_set(event.xbutton.button, 1);
    });
    register_handler(InputEventType::button_release, [](InputManagerImplementationPtr im, input_event_reference event, previous_event_reference previous) {
        auto actual = std::static_pointer_cast<GameInputManager>(im);
        return actual->try_set(event.xbutton.button, 0);
    });
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

}