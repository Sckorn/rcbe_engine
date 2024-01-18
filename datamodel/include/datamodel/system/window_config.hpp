#ifndef RCBE_ENGINE_WINDOW_CONFIG_HPP
#define RCBE_ENGINE_WINDOW_CONFIG_HPP

#include <functional>
#include <string>
#include <unordered_map>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

#include <nlohmann/json_fwd.hpp>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

namespace rcbe::core {

namespace window {
using UunmapHandlerType = std::function<void()>;
using ConfigureHandlerType = std::function<void()>;
}// namespace window

struct window_config {
    enum class WindowType {
        gl_rendering_window,
        drawing_window,
        unknown = 255
    };

    std::string caption;
    math::Vector2d position;
    core::IntegralDimensions size;
    visual::RGBAColor background_color;
    bool fullscreen = false;
    bool process_input = false;
    bool editor = false;
    WindowType type = WindowType ::unknown;
    std::string input_scheme;
};

using WindowTypeBimap = boost::bimap<std::string, window_config::WindowType>;

static const WindowTypeBimap str_to_wnd_t_ = boost::assign::list_of<WindowTypeBimap::relation>("gl_renderer_window", window_config::WindowType ::gl_rendering_window)("drawing_window", window_config::WindowType::drawing_window)("unknown", window_config::WindowType ::unknown);

static std::string str_from_window_type(window_config::WindowType type) {
    auto it = str_to_wnd_t_.right.find(type);
    if (it != str_to_wnd_t_.right.end()) {
        return it->second;
    }
    return "uknown";
}

static window_config::WindowType wnd_type_from_string(const std::string &type) {
    auto it = str_to_wnd_t_.left.find(type);
    if (it != str_to_wnd_t_.left.end()) {
        return it->second;
    }

    return window_config::WindowType ::unknown;
}
}// namespace rcbe::core

namespace nlohmann {
template <>
struct adl_serializer<rcbe::core::window_config> {
    static void to_json(json &j, const rcbe::core::window_config &config);
    static void from_json(const json &j, rcbe::core::window_config &config);
};
}// namespace nlohmann
#endif//RCBE_ENGINE_WINDOW_CONFIG_HPP
