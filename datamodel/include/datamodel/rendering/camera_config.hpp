#ifndef RCBE_ENGINE_CAMERA_CONFIG_HPP
#define RCBE_ENGINE_CAMERA_CONFIG_HPP

#include <nlohmann/json_fwd.hpp>
#include <rcbe-engine/datamodel/math/math_constants.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::rendering {
struct camera_config {
    math::Vector3d camera_position;
    math::Vector3d camera_lookat;
    math::Vector3d camera_up = math::WORLD_UP;
};
}

namespace nlohmann {
template <>
struct adl_serializer<rcbe::rendering::camera_config> {
    static void to_json(nlohmann::json &j, const rcbe::rendering::camera_config &c);
    static void from_json(const nlohmann::json &j, rcbe::rendering::camera_config &c);
};
}

#endif //RCBE_ENGINE_CAMERA_CONFIG_HPP
