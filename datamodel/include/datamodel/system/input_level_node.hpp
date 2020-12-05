#ifndef RCBE_ENGINE_INPUT_LEVEL_NODE_HPP
#define RCBE_ENGINE_INPUT_LEVEL_NODE_HPP

#include <string>

#include <rcbe-engine/datamodel/system/input_system_types.hpp>

#include <nlohmann/json_fwd.hpp>

namespace rcbe::core {
struct high_level_node {
    core::InputValueType type = core::InputValueType::unknown;
    std::string name;
    int value = 0; //TODO: replace by ValueWrapper
};

using HighLevelInputNode = high_level_node;

struct low_level_node {
    std::string name;
    int multiplier;
    int keycode;
};

using LowLevelInputNode = low_level_node;
}

namespace nlohmann {
template<>
struct adl_serializer<rcbe::core::high_level_node> {
static void to_json(json& j, const rcbe::core::high_level_node& hln);
static void from_json(const json& j, rcbe::core::high_level_node& hln);
};

template <>
struct adl_serializer<rcbe::core::low_level_node> {
static void to_json(json& j, const rcbe::core::low_level_node& lln);
static void from_json(const json& j, rcbe::core::low_level_node& lln);
};
}

#endif //RCBE_ENGINE_INPUT_LEVEL_NODE_HPP
