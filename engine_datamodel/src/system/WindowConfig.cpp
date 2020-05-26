#include <data_types/system/WindowConfig.hpp>

#include <nlohmann/json.hpp>

namespace nlohmann {
void adl_serializer<rcbe::core::WindowConfig>::to_json(json& j, const rcbe::core::WindowConfig& config) {
    j = {
            {"type", rcbe::core::str_from_window_type(config.type)},
            {"size", config.size},
            {"background_color", config.background_color},
            {"caption", config.caption},
            {"position", config.position},
            {"fullsreen", config.fullsreen}
    };
}

void adl_serializer<rcbe::core::WindowConfig>::from_json(const json& j, rcbe::core::WindowConfig& config) {
    config.type = rcbe::core::wnd_type_from_string(j.at("type").get<std::string>());
    config.size = j.at("size").get<rcbe::core::Dimensions>();
    config.fullsreen = j.at("fullscreen").get<bool>();
    config.position = j.at("position").get<rcbe::math::Vector2d>();
    config.caption = j.at("caption").get<std::string>();
    config.background_color = j.at("background_color").get<rcbe::visual::RGBAColor>();
}
}
