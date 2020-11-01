#ifndef RCBE_ENGINE_GAMEINPUTMANAGER_HPP
#define RCBE_ENGINE_GAMEINPUTMANAGER_HPP

#include <memory>

#include <nlohmann/json.hpp>

#include <core/InputManagerImplementation.hpp>
#include <data_types/system/InputScheme.hpp>

namespace rcbe::core {
class GameInputManager;

using GameInputManagerPtr = std::shared_ptr<GameInputManager>;
using GameInputManagerConstPtr = std::shared_ptr<const GameInputManager>;

class GameInputManager final : public InputManagerImplementation {
public:
    GameInputManager() = delete;
    explicit GameInputManager(nlohmann::json&& j);
    ~GameInputManager() = default;
    GameInputManager(const GameInputManager& other) = delete;
    GameInputManager(GameInputManager&& other) = default;
    GameInputManager& operator=(const GameInputManager& other) = delete;
    GameInputManager& operator=(GameInputManager&& other) = default;

    [[nodiscard]] auto get(const std::string& input_event_name) const {
        return scheme_.get(input_event_name);
    }

     static GameInputManagerPtr create(nlohmann::json&& j) {
        return std::make_shared<GameInputManager>(std::move(j));
    }

    bool try_set(int keycode, int value);

private:

    InputScheme scheme_;
};
}

#endif //RCBE_ENGINE_GAMEINPUTMANAGER_HPP
