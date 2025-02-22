#include <nlohmann/json.hpp>

#include <rcbe-engine/datamodel/system/window_config.hpp>

namespace nlohmann {
void adl_serializer<rcbe::core::window_config>::to_json(json &j, const rcbe::core::window_config &config) {
    j = {
        {"type", rcbe::core::str_from_window_type(config.type)},
        {"size", config.size},
        {"background_color", config.background_color},
        {"caption", config.caption},
        {"position", config.position},
        {"fullscreen", config.fullscreen},
        {"editor", config.editor},
        {"process_input", config.process_input},
        {"input_scheme", config.input_scheme}};
}

void adl_serializer<rcbe::core::window_config>::from_json(const json &j, rcbe::core::window_config &config) {
    config.type = rcbe::core::wnd_type_from_string(j.at("type").get<std::string>());
    config.size = j.at("size").get<rcbe::core::IntegralDimensions>();
    config.fullscreen = j.at("fullscreen").get<bool>();
    config.position = j.at("position").get<rcbe::math::Vector2d>();
#ifdef __linux__
    config.caption = j.at("caption").get<std::string>();
#endif
#ifdef _WIN32
    auto cstring = j.at("caption").get<std::string>();
    config.caption = std::wstring(cstring.begin(), cstring.end()); /// TODO: not secure, address when codecvt problem is addressed @sckorn
#endif
    config.background_color = j.at("background_color").get<rcbe::visual::RGBAColor>();
    config.process_input = j.at("process_input").get<bool>();
    config.editor = j.at("editor").get<bool>();
    config.input_scheme = j.value<std::string>("input_scheme", "");
}
}// namespace nlohmann
