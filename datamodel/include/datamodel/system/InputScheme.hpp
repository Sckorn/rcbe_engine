#ifndef RCBE_ENGINE_INPUTSCHEME_HPP
#define RCBE_ENGINE_INPUTSCHEME_HPP

#include <string>
#include <unordered_map>

#include <nlohmann/json_fwd.hpp>

#include <rcbe-engine/datamodel/system/input_level_node.hpp>
#include <rcbe-engine/datamodel/system/input_system_types.hpp>

namespace rcbe::core {
class InputDeviceScheme {
public:

    using HighLevelEvents = std::unordered_map<std::string, HighLevelInputNode>;
    using LowLevelEvents = std::unordered_map<int, LowLevelInputNode>;

    explicit InputDeviceScheme(nlohmann::json &&j);
    ~InputDeviceScheme() = default;

    [[nodiscard]] const HighLevelEvents &highlevel() const noexcept;
    [[nodiscard]] HighLevelEvents &highlevel();
    [[nodiscard]] const LowLevelEvents &lowlevel() const noexcept;

private:

    HighLevelEvents highlevel_;
    LowLevelEvents lowlevel_;
};

class InputScheme {
public:

    explicit InputScheme(nlohmann::json &&j);
    ~InputScheme() = default;

    [[nodiscard]] auto get(const std::string &key) const {
        for (const auto &[key_, dev] : device_inputs_) {
            const auto &high = dev.highlevel();
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
        for (const auto &[key_, dev] : device_inputs_) {
            const auto &low = dev.lowlevel();
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
}// namespace rcbe::core

#endif//RCBE_ENGINE_INPUTSCHEME_HPP
