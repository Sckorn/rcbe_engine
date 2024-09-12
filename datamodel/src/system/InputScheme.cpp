#include <nlohmann/json.hpp>

#include <rcbe-engine/datamodel/system/InputScheme.hpp>

namespace rcbe::core {
R_PUBLIC_API InputDeviceScheme::InputDeviceScheme(nlohmann::json &&j) {
    const auto highlevel = j.at("highlevel");

    for (auto it = highlevel.begin(); it != highlevel.end(); ++it) {
        high_level_node node = *it;
        highlevel_.insert({node.name, node});
    }

    const auto lowlevel = j.at("lowlevel");

    for (auto it = lowlevel.begin(); it != lowlevel.end(); ++it) {
        low_level_node node = *it;
        lowlevel_.insert({node.keycode, node});
    }
}

R_PUBLIC_API const InputDeviceScheme::HighLevelEvents &InputDeviceScheme::highlevel() const noexcept {
    return highlevel_;
}

R_PUBLIC_API InputDeviceScheme::HighLevelEvents &InputDeviceScheme::highlevel() {
    return highlevel_;
}

R_PUBLIC_API const InputDeviceScheme::LowLevelEvents &InputDeviceScheme::lowlevel() const noexcept {
    return lowlevel_;
}

R_PUBLIC_API InputScheme::InputScheme(nlohmann::json &&j) {
    for (auto it = j.begin(); it != j.end(); ++it) {
        device_inputs_.insert({it.key(), InputDeviceScheme(std::move(it.value()))});
    }
}

R_PUBLIC_API void InputScheme::set(const int key, int value) {
    value = std::clamp(value, 0, 1);
    for (auto &[key_, dev] : device_inputs_) {
        const auto &low = dev.lowlevel();
        auto it = low.find(key);
        if (it == low.end()) continue;
        auto &high = dev.highlevel();
        auto &high_node = high.at(it->second.name);
        high_node.value = (high_node.type == InputValueType::axis) ? value * it->second.multiplier : value;
        break;
    }
}
}// namespace rcbe::core