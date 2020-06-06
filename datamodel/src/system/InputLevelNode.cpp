#include <data_types/system/InputLevelNode.hpp>

#include <nlohmann/json.hpp>

namespace nlohmann {
void adl_serializer<rcbe::core::HighLevelNode>::to_json(json& j, const rcbe::core::HighLevelNode& hln) {
    j = {
            {"type", rcbe::core::str_from_input_value_type(hln.type)},
            {"name", hln.name}
    };
}

void adl_serializer<rcbe::core::HighLevelNode>::from_json(const json& j, rcbe::core::HighLevelNode& hln) {
    hln.type = rcbe::core::input_value_type_from_string(j.at("type").get<std::string>());
    hln.name = j.at("name").get<std::string>();
}

void adl_serializer<rcbe::core::LowLevelNode>::to_json(json& j, const rcbe::core::LowLevelNode& lln) {
    j = {
            {"name", lln.name},
            {"keycode", lln.keycode},
            {"multiplier", lln.multiplier}
    };
}

void adl_serializer<rcbe::core::LowLevelNode>::from_json(const json& j, rcbe::core::LowLevelNode& lln) {
    lln.keycode = j.at("keycode").get<int>();
    lln.multiplier = j.at("multiplier").get<int>();
    lln.name = j.at("name").get<std::string>();
}
}