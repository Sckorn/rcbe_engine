#ifndef RCBE_ENGINE_INPUTLEVELNODE_HPP
#define RCBE_ENGINE_INPUTLEVELNODE_HPP

#include <string>

#include <data_types/system/InputSystemTypes.hpp>

#include <nlohmann/json_fwd.hpp>

namespace rcbe::core {
struct HighLevelNode {
    core::InputValueType type = core::InputValueType::unknown;
    std::string name;
    int value = 0; //TODO: replace by ValueWrapper
};

using high_level_input_node = HighLevelNode;

struct LowLevelNode {
    std::string name;
    int multiplier;
    int keycode;
};

using low_level_input_node = LowLevelNode;
}

namespace nlohmann {
template<>
struct adl_serializer<rcbe::core::HighLevelNode> {
static void to_json(json& j, const rcbe::core::HighLevelNode& hln);
static void from_json(const json& j, rcbe::core::HighLevelNode& hln);
};

template <>
struct adl_serializer<rcbe::core::LowLevelNode> {
static void to_json(json& j, const rcbe::core::LowLevelNode& lln);
static void from_json(const json& j, rcbe::core::LowLevelNode& lln);
};
}

#endif //RCBE_ENGINE_INPUTLEVELNODE_HPP
