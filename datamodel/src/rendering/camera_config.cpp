#include <nlohmann/json.hpp>

#include <rcbe-engine/datamodel/rendering/camera_config.hpp>
#include <rcbe-engine/datamodel/math/math_constants.hpp>

namespace nlohmann {
void adl_serializer<rcbe::rendering::camera_config>::to_json(
        nlohmann::json &j,
        const rcbe::rendering::camera_config &c
) {
    j = {
            {"camera_position", c.camera_position},
            {"camera_lookat", c.camera_lookat},
            {"camera_up", c.camera_up},
            {"maximum_fov", c.max_fov},
            {"minimum_fov", c.min_fov},
            {"default_fov", c.def_fov}
    };
}

void adl_serializer<rcbe::rendering::camera_config>::from_json(
        const nlohmann::json &j,
        rcbe::rendering::camera_config &c
) {
    c.camera_position = j.at("camera_position").get<rcbe::math::Vector3d>();
    c.camera_lookat = j.at("camera_lookat").get<rcbe::math::Vector3d>();
    c.camera_up = j.value<rcbe::math::Vector3d>("camera_up", rcbe::math::WORLD_UP);
    c.max_fov = j.at("maximum_fov").get<rcbe::core::EngineScalar>();
    c.min_fov = j.at("minimum_fov").get<rcbe::core::EngineScalar>();
    c.def_fov = j.at("default_fov").get<rcbe::core::EngineScalar>();
}
}
