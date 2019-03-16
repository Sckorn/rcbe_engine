#ifndef RCBE_ENGINE_TOOLKITGUICONFIG_H
#define RCBE_ENGINE_TOOLKITGUICONFIG_H

#include <string>
#include <unordered_set>

#include "json/json.h"

namespace rcbe::config
{
struct WindowData
{
  std::string id;
  std::string file_name;
  std::vector<WindowData> children;
};

void to_json(nlohmann::json &json, const WindowData &conf);
void from_json(const nlohmann::json &json, WindowData &conf);

struct WindowManagerConfig
{
  std::string widgets_dir_path;
  WindowData widgets;
};

void to_json(nlohmann::json &json, const WindowManagerConfig &conf);
void from_json(const nlohmann::json &json, WindowManagerConfig &conf);
}

#endif //RCBE_ENGINE_TOOLKITGUICONFIG_H
