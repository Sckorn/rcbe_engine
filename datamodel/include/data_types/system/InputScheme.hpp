#ifndef RCBE_ENGINE_INPUTSCHEME_HPP
#define RCBE_ENGINE_INPUTSCHEME_HPP

#include <unordered_map>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <data_types/system/InputSystemTypes.hpp>
#include <data_types/system/InputLevelNode.hpp>

namespace rcbe::core {
class InputDeviceScheme {
public:
    using high_level_events = std::unordered_map<std::string, high_level_input_node>;
    using low_level_events = std::unordered_map<int, low_level_input_node>;

    explicit InputDeviceScheme(nlohmann::json&& j);
    ~InputDeviceScheme() = default;

    [[nodiscard]] const high_level_events& highlevel() const;
    [[nodiscard]] high_level_events& highlevel();
    [[nodiscard]] const low_level_events& lowlevel() const;

private:
    high_level_events highlevel_;
    low_level_events lowlevel_;
};

class InputScheme {
public:
    explicit InputScheme(nlohmann::json&& j);
    ~InputScheme() = default;

    [[nodiscard]] auto get(const std::string& key) const {
        for (const auto&[key_, dev] : device_inputs_) {
            const auto& high = dev.highlevel();
            auto it = high.find(key);
            if (it != high.end()) {
                return it->second.value;
            } else {
                continue;
            }
        }

        return 0;
    }

    [[nodiscard]] auto get(const int key) const {
        for (const auto&[key_, dev] : device_inputs_) {
            const auto& low = dev.lowlevel();
            auto it = low.find(key);
            if (it != low.end()) {
                return get(it->second.name);
            } else {
                continue;
            }
        }

        return 0;
    }

    void set(int key, int value);

private:
    std::unordered_map<std::string, InputDeviceScheme> device_inputs_;
};
}

#endif //RCBE_ENGINE_INPUTSCHEME_HPP
