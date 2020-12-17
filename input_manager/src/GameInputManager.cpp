#include <core/GameInputManager.hpp>

#include <nlohmann/json.hpp>

namespace rcbe::core {
GameInputManager::GameInputManager(nlohmann::json&& j)
:
scheme_(std::move(j))
{
    register_handler(InputEventType::key_press, [](InputManagerImplementation& im, input_event_reference event, previous_event_reference previous) {
        auto * ptr = std::addressof(im);
        auto actual = dynamic_cast<GameInputManager*>(ptr);
        if (!actual)
            throw std::runtime_error("Can't cast to proper input manager type!");
        return actual->try_set(event.xkey.keycode, 1);
    });
    register_handler(InputEventType::key_release, [](InputManagerImplementation& im, input_event_reference event, previous_event_reference previous) {
        auto * ptr = std::addressof(im);
        auto actual = dynamic_cast<GameInputManager*>(ptr);
        if (!actual)
            throw std::runtime_error("Can't cast to proper input manager type!");
        return actual->try_set(event.xkey.keycode, 0);
    });
    register_handler(InputEventType::button_press, [](InputManagerImplementation& im, input_event_reference event, previous_event_reference previous) {
        auto * ptr = std::addressof(im);
        auto actual = dynamic_cast<GameInputManager*>(ptr);
        if (!actual)
            throw std::runtime_error("Can't cast to proper input manager type!");
        return actual->try_set(event.xbutton.button, 1);
    });
    register_handler(InputEventType::button_release, [](InputManagerImplementation& im, input_event_reference event, previous_event_reference previous) {
        auto * ptr = std::addressof(im);
        auto actual = dynamic_cast<GameInputManager*>(ptr);
        if (!actual)
            throw std::runtime_error("Can't cast to proper input manager type!");
        return actual->try_set(event.xbutton.button, 0);
    });
}

bool GameInputManager::try_set(const int keycode, const int value) const {
    try {
        scheme_.set(keycode, value);
    } catch (const std::exception& ex) {
        BOOST_LOG_TRIVIAL(debug) << "Exception in handler: " << ex.what();
        return false;
    }

    return true;
}
}