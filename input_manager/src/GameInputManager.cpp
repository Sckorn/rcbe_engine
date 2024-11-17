#include <nlohmann/json.hpp>

#include <rcbe-engine/core/GameInputManager.hpp>

namespace rcbe::core {
GameInputManager::GameInputManager(nlohmann::json &&j)
    : scheme_(std::move(j)) {
    registerHandler(InputEventType::key_press,
                    [](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                        auto *ptr = std::addressof(im);
                        auto actual = dynamic_cast<GameInputManager *>(ptr);
                        if (!actual)
                            throw std::runtime_error("Can't cast to proper input manager type!");
#ifdef __linux__
                        return actual->trySet(event.xkey.keycode, 1);
#endif
#ifdef _WIN32
                        return actual->trySet(event.wparam, 1);
#endif
                    });
    registerHandler(InputEventType::key_release,
                    [](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                        auto *ptr = std::addressof(im);
                        auto actual = dynamic_cast<GameInputManager *>(ptr);
                        if (!actual)
                            throw std::runtime_error("Can't cast to proper input manager type!");
#ifdef __linux__
                        return actual->trySet(event.xkey.keycode, 0);
#endif
#ifdef _WIN32
                        return actual->trySet(event.wparam, 0);
#endif
                    });
#ifdef __linux__
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
#endif
#ifdef _WIN32
    registerHandler(InputEventType::left_button_press,
                    [](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                        auto *ptr = std::addressof(im);
                        auto actual = dynamic_cast<GameInputManager *>(ptr);
                        if (!actual)
                            throw std::runtime_error("Can't cast to proper input manager type!");
                        return actual->trySet(event.type, 1);
                    });
    registerHandler(InputEventType::left_button_release,
                    [](InputManagerImplementation &im, InputEventReference event, PreviousEventReference previous) {
                        auto *ptr = std::addressof(im);
                        auto actual = dynamic_cast<GameInputManager *>(ptr);
                        if (!actual)
                            throw std::runtime_error("Can't cast to proper input manager type!");
                        return actual->trySet(event.type, 0);
                    });
#endif
}

bool GameInputManager::trySet(const int keycode, const int value) const {
    try {
        scheme_.set(keycode, value);
    } catch (const std::exception &ex) {
        RDMN_LOG(RDMN_LOG_DEBUG) << "Exception in handler: " << ex.what();
        return false;
    }

    return true;
}
}// namespace rcbe::core