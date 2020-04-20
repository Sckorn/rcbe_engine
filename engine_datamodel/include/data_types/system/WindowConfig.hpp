#ifndef RCBE_ENGINE_WINDOWCONFIG_HPP
#define RCBE_ENGINE_WINDOWCONFIG_HPP

#include <string>
#include <functional>
#include <unordered_map>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

#include <data_types/core/Dimensions.hpp>
#include <data_types/math/Vector.hpp>
#include <data_types/visual/RGBAColor.hpp>

#include <nlohmann/json_fwd.hpp>

namespace rcbe::core {

namespace window {
using unmap_handler_t = std::function<void()>;
using configure_handler_t = std::function<void()>;
}

struct WindowConfig {
    enum class WindowType {
        GL_RENDERING_WINDOW,
        DRAWING_WINDOW,
        UNKNOWN = 255
    };

    std::string caption;
    math::Vector2d position;
    core::Dimensions size;
    visual::RGBAColor background_color;
    bool fullsreen = false;
    WindowType type = WindowType ::UNKNOWN;
};

using WindowTypeBimap = boost::bimap<std::string, WindowConfig::WindowType>;

static const WindowTypeBimap str_to_wnd_t_ =  boost::assign::list_of<WindowTypeBimap::relation>
        ("gl_renderer_window", WindowConfig::WindowType ::GL_RENDERING_WINDOW)
        ("drawing_window", WindowConfig::WindowType::DRAWING_WINDOW)
        ("unknown", WindowConfig::WindowType ::UNKNOWN)
;

static std::string str_from_window_type(WindowConfig::WindowType type) {
    auto it = str_to_wnd_t_.right.find(type);
    if (it != str_to_wnd_t_.right.end())
    {
        return it->second;
    }
    return "uknown";
}

static WindowConfig::WindowType wnd_type_from_string(const std::string& type) {
    auto it = str_to_wnd_t_.left.find(type);
    if (it != str_to_wnd_t_.left.end())
    {
        return it->second;
    }

    return WindowConfig::WindowType ::UNKNOWN;
}
}

namespace nlohmann {
template <>
struct adl_serializer<rcbe::core::WindowConfig> {
static void to_json(json& j, const rcbe::core::WindowConfig& config);
static void from_json(const json& j, rcbe::core::WindowConfig& config);
};
}
#endif //RCBE_ENGINE_WINDOWCONFIG_HPP
