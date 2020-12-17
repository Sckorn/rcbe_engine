#include <rcbe-engine/datamodel/system/input_level_node.hpp>

#include <nlohmann/json.hpp>

namespace nlohmann {
void adl_serializer<rcbe::core::high_level_node>::to_json(json& j, const rcbe::core::high_level_node& hln) {
    j = {
            {"type", rcbe::core::str_from_input_value_type(hln.type)},
            {"name", hln.name}
    };
}

void adl_serializer<rcbe::core::high_level_node>::from_json(const json& j, rcbe::core::high_level_node& hln) {
    hln.type = rcbe::core::input_value_type_from_string(j.at("type").get<std::string>());
    hln.name = j.at("name").get<std::string>();
}

void adl_serializer<rcbe::core::low_level_node>::to_json(json& j, const rcbe::core::low_level_node& lln) {
    j = {
            {"name", lln.name},
            {"keycode", lln.keycode},
            {"multiplier", lln.multiplier}
    };
}

void adl_serializer<rcbe::core::low_level_node>::from_json(const json& j, rcbe::core::low_level_node& lln) {
    lln.keycode = j.at("keycode").get<int>();
    lln.multiplier = j.at("multiplier").get<int>();
    lln.name = j.at("name").get<std::string>();
}
}