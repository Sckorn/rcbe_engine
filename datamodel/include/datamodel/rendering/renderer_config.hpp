#ifndef RCBE_ENGINE_RENDERER_CONFIG_HPP
#define RCBE_ENGINE_RENDERER_CONFIG_HPP

#include <string>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

#include <nlohmann/json_fwd.hpp>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace rcbe::rendering {
enum class RendererType {
    hardware,
    software,
    unknown
};

using RendererTypeBimap = boost::bimap<std::string, RendererType>;

static const RendererTypeBimap rtb_table = boost::assign::list_of<RendererTypeBimap::relation>
        ("hardware", RendererType::hardware)
        ("software", RendererType::software)
        ;

static std::string str_from_renderer_type(RendererType type) {
    auto it = rtb_table.right.find(type);
    if (it != rtb_table.right.end())
    {
        return it->second;
    }
    return "uknown";
}

static RendererType renderer_type_from_string(const std::string& type) {
    auto it = rtb_table.left.find(type);
    if (it != rtb_table.left.end())
    {
        return it->second;
    }

    return RendererType ::unknown;
}

struct renderer_config
{
    std::string window_caption;
    rcbe::core::IntegralDimensions initial_dimensions;
    bool resizable;
    rcbe::math::Vector2d initial_window_position;
    rcbe::visual::RGBAColor clear_color;
    RendererType renderer_type;
    bool debug = true;
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
