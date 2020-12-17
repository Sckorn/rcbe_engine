#include <core/GameInputManager.hpp>

#include <nlohmann/json.hpp>

namespace rcbe::core {
GameInputManager::GameInputManager(nlohmann::json&& j)
:
scheme_(std::move(j))
{
    registerHandler(InputEventType::key_press,
                    [](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                        auto *ptr = std::addressof(im);
                        auto actual = dynamic_cast<GameInputManager *>(ptr);
                        if (!actual)
                            throw std::runtime_error("Can't cast to proper input manager type!");
                        return actual->trySet(event.xkey.keycode, 1);
                    });
    registerHandler(InputEventType::key_release,
                    [](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                        auto *ptr = std::addressof(im);
                        auto actual = dynamic_cast<GameInputManager *>(ptr);
                        if (!actual)
                            throw std::runtime_error("Can't cast to proper input manager type!");
                        return actual->trySet(event.xkey.keycode, 0);
                    });
    registerHandler(InputEventType::button_press,
                    [](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                        auto *ptr = std::addressof(im);
                        auto actual = dynamic_cast<GameInputManager *>(ptr);
                        if (!actual)
                            throw std::runtime_error("Can't cast to proper input manager type!");
                        return actual->trySet(event.xbutton.button, 1);
                    });
    registerHandler(InputEventType::button_release,
                    [](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                        auto *ptr = std::addressof(im);
                        auto actual = dynamic_cast<GameInputManager *>(ptr);
                        if (!actual)
                            throw std::runtime_error("Can't cast to proper input manager type!");
                        return actual->trySet(event.xbutton.button, 0);
                    });
}

bool GameInputManager::trySet(const int keycode, const int value) const {
    try {
        scheme_.set(keycode, value);
    } catch (const std::exception& ex) {
        BOOST_LOG_TRIVIAL(debug) << "Exception in handler: " << ex.what();
        return false;
    }

    return true;
}
}