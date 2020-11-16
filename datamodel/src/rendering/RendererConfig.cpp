#include <data_types/rendering/RendererConfig.hpp>

#include <nlohmann/json.hpp>

namespace nlohmann {
void adl_serializer<rcbe::rendering::RendererConfig>::to_json(json &j, const rcbe::rendering::RendererConfig& conf) {
    j = {
            {"window_caption", conf.window_caption},
            {"resizable", conf.resizable},
            {"clear_color", conf.clear_color},
            {"initial_dimensions", conf.initial_dimensions},
            {"initial_window_position", conf.initial_window_position},
            {"camera_position", conf.camera_position},
            {"camera_lookat", conf.camera_lookat},
    };
}

void adl_serializer<rcbe::rendering::RendererConfig>::from_json(const json& j, rcbe::rendering::RendererConfig &conf) {
    conf.window_caption = j.at("window_caption").get<std::string>();
    conf.initial_window_position = j.at("initial_window_position").get<rcbe::math::Vector2d>();
    conf.initial_dimensions = j.at("initial_dimensions").get<rcbe::core::Dimensions>();
    conf.clear_color = j.at("clear_color").get<rcbe::visual::RGBAColor>();
    conf.resizable = j.at("resizable").get<bool>();
    conf.camera_position = j.at("camera_position").get<rcbe::math::Vector3d>();
    conf.camera_lookat = j.at("camera_lookat").get<rcbe::math::Vector3d>();
}
}
