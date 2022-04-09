#include <rcbe-engine/datamodel/rendering/renderer_config.hpp>

#include <nlohmann/json.hpp>

namespace nlohmann {
void adl_serializer<rcbe::rendering::renderer_config>::to_json(json &j, const rcbe::rendering::renderer_config& conf) {
    j = {
            {"window_caption", conf.window_caption},
            {"resizable", conf.resizable},
            {"clear_color", conf.clear_color},
            {"initial_dimensions", conf.initial_dimensions},
            {"initial_window_position", conf.initial_window_position},
            {"renderer_type", rcbe::rendering::str_from_renderer_type(conf.renderer_type)},
    };
}

void adl_serializer<rcbe::rendering::renderer_config>::from_json(const json& j, rcbe::rendering::renderer_config &conf) {
    conf.window_caption = j.at("window_caption").get<std::string>();
    conf.initial_window_position = j.at("initial_window_position").get<rcbe::math::Vector2d>();
    conf.initial_dimensions = j.at("initial_dimensions").get<rcbe::core::IntegralDimensions>();
    conf.clear_color = j.at("clear_color").get<rcbe::visual::RGBAColor>();
    conf.resizable = j.at("resizable").get<bool>();
    conf.renderer_type = rcbe::rendering::renderer_type_from_string(j.at("renderer_type").get<std::string>());
}
}
