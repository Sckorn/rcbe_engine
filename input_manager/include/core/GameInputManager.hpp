#ifndef RCBE_ENGINE_GAMEINPUTMANAGER_HPP
#define RCBE_ENGINE_GAMEINPUTMANAGER_HPP

#include <nlohmann/json.hpp>

#include <core/InputManagerImplementation.hpp>
#include <rcbe-engine/datamodel/system/InputScheme.hpp>

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

    static GameInputManager create(nlohmann::json&& j) {
        return GameInputManager(std::move(j));
    }

    [[nodiscard]] auto get(const std::string& input_event_name) const {
        return scheme_.get(input_event_name);
    }

    using InputManagerImplementation::tryProcessEvent;
    using InputManagerImplementation::getValue;

private:
    bool trySet(int keycode, int value) const;

    mutable InputScheme scheme_;
};
}

#endif //RCBE_ENGINE_GAMEINPUTMANAGER_HPP
