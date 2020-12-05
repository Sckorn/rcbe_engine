#ifndef RCBE_ENGINE_RENDERER_CONFIG_HPP
#define RCBE_ENGINE_RENDERER_CONFIG_HPP

#include <string>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

#include <nlohmann/json_fwd.hpp>

namespace rcbe::rendering {
struct renderer_config
{
    std::string window_caption;
    rcbe::core::Dimensions initial_dimensions;
    bool resizable;
    rcbe::math::Vector2d initial_window_position;
    rcbe::visual::RGBAColor clear_color;
};

}

namespace nlohmann {
template <>
struct adl_serializer<rcbe::rendering::renderer_config> {
    static void to_json(json &j, const rcbe::rendering::renderer_config& conf);
    static void from_json(const json& j, rcbe::rendering::renderer_config &conf);
};
}

#endif //RCBE_ENGINE_RENDERER_CONFIG_HPP
