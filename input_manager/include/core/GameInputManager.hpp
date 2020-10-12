#ifndef RCBE_ENGINE_GAMEINPUTMANAGER_HPP
#define RCBE_ENGINE_GAMEINPUTMANAGER_HPP

#include <nlohmann/json_fwd.hpp>

#include <core/InputManagerImplementation.hpp>
#include <data_types/system/InputScheme.hpp>

namespace rcbe::core {
class GameInputManager final : protected InputManagerImplementation {
public:
    GameInputManager() = delete;
    explicit GameInputManager(nlohmann::json&& j);
    ~GameInputManager() = default;
    GameInputManager(const GameInputManager& other) = delete;
    GameInputManager(GameInputManager&& other) = default;
    GameInputManager& operator=(const GameInputManager& other) = delete;
    GameInputManager& operator=(GameInputManager&& other) = default;

    bool try_process_event(input_event_reference event);

    void init();

    [[nodiscard]] auto get(const std::string& input_event_name) const {
        return scheme_.get(input_event_name);
    }

private:
    bool try_set(int keycode, int value);

    bool methods_initialized_ = false;
    InputScheme scheme_;
};
}

#endif //RCBE_ENGINE_GAMEINPUTMANAGER_HPP
