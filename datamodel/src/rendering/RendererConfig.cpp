#include <data_types/rendering/RendererConfig.hpp>

#include <nlohmann/json.hpp>

namespace nlohmann {
void adl_serializer<rcbe::rendering::RendererConfig>::to_json(json &j, const rcbe::rendering::RendererConfig& conf) {
    j = {
            {"window_caption", conf.window_caption},
            {"camera_zoom", conf.camera_zoom},
            {"resizable", conf.resizable},
            {"clear_color", conf.clear_color},
            {"initial_dimensions", conf.initial_dimensions},
            {"initial_position", conf.initial_position}
    };
}

void adl_serializer<rcbe::rendering::RendererConfig>::from_json(const json& j, rcbe::rendering::RendererConfig &conf) {
    conf.window_caption = j.at("window_caption").get<std::string>();
    conf.initial_position = j.at("initial_position").get<rcbe::math::Vector2d>();
    conf.initial_dimensions = j.at("initial_dimensions").get<rcbe::core::Dimensions>();
    conf.clear_color = j.at("clear_color").get<rcbe::visual::RGBAColor>();
    conf.camera_zoom = j.at("camera_zoom").get<double>();
    conf.resizable = j.at("resizable").get<bool>();
}
}
