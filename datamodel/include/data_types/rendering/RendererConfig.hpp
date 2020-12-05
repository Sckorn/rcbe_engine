#ifndef RCBE_ENGINE_RENDERERCONFIG_HPP
#define RCBE_ENGINE_RENDERERCONFIG_HPP

#include <string>

#include <data_types/core/Dimensions.hpp>
#include <data_types/math/Vector.hpp>
#include <data_types/visual/RGBAColor.hpp>

#include <nlohmann/json_fwd.hpp>

namespace rcbe::rendering {
struct RendererConfig
{
    std::string window_caption;
    rcbe::core::Dimensions initial_dimensions;
    bool resizable;
    rcbe::math::Vector2d initial_window_position;
    rcbe::visual::RGBAColor clear_color;
    //TODO: introduce camera config, move these there
    rcbe::math::Vector3d camera_position;
    rcbe::math::Vector3d camera_lookat;
};

}

namespace nlohmann {
template <>
struct adl_serializer<rcbe::rendering::RendererConfig> {
    static void to_json(json &j, const rcbe::rendering::RendererConfig& conf);
    static void from_json(const json& j, rcbe::rendering::RendererConfig &conf);
};
}

#endif //RCBE_ENGINE_RENDERERCONFIG_HPP
