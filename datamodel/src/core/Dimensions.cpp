#include <nlohmann/json.hpp>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>

namespace nlohmann {
R_PUBLIC_API void adl_serializer<rcbe::core::IntegralDimensions>::to_json(json &j, const rcbe::core::IntegralDimensions &dim) {
    j = {
        {"width", dim.width},
        {"height", dim.height}};
}

R_PUBLIC_API void adl_serializer<rcbe::core::IntegralDimensions>::from_json(const json &j, rcbe::core::IntegralDimensions &dim) {
    dim.width = j.at("width").get<rcbe::core::IntegralDimensions::ValueType>();
    dim.height = j.at("height").get<rcbe::core::IntegralDimensions::ValueType>();
}
}// namespace nlohmann