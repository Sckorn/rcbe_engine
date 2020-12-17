#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <nlohmann/json.hpp>

namespace nlohmann {
void adl_serializer<rcbe::core::Dimensions>::to_json(json& j, const rcbe::core::Dimensions& dim) {
    j = {
            {"width", dim.width},
            {"height", dim.height}
    };
}

void adl_serializer<rcbe::core::Dimensions>::from_json(const json& j, rcbe::core::Dimensions& dim) {
    dim.width = j.at("width").get<rcbe::core::Dimensions::ValueType>();
    dim.height = j.at("height").get<rcbe::core::Dimensions::ValueType>();
}
}