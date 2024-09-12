#ifndef RCBE_ENGINE_RENDERER_CONFIG_HPP
#define RCBE_ENGINE_RENDERER_CONFIG_HPP

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

#include <rdmn-engine/public_api.hpp>

namespace rcbe::rendering {
enum class RendererType {
    hardware,
    software,
    unknown
};

static std::string str_from_renderer_type(RendererType type) {
    switch (type)
    {
    case RendererType::hardware:
        return "hardware";
    case RendererType::software:
        return "software";
    case RendererType::unknown:
    default:
        return "uknown";
    }
}

static RendererType renderer_type_from_string(const std::string &type) {
    if (type == "hardware")
        return RendererType::hardware;

    if (type == "software")
        return RendererType::software;

    return RendererType ::unknown;
}

struct renderer_config {
    std::string window_caption;
    rcbe::core::IntegralDimensions initial_dimensions;
    bool resizable;
    rcbe::math::Vector2d initial_window_position;
    rcbe::visual::RGBAColor clear_color;
    RendererType renderer_type;
    bool debug = true;
};

}// namespace rcbe::rendering

namespace nlohmann {
template <>
struct adl_serializer<rcbe::rendering::renderer_config> {
    R_PUBLIC_API static void to_json(json &j, const rcbe::rendering::renderer_config &conf);
    R_PUBLIC_API static void from_json(const json &j, rcbe::rendering::renderer_config &conf);
};
}// namespace nlohmann

#endif//RCBE_ENGINE_RENDERER_CONFIG_HPP
