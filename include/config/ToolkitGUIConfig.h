#ifndef RCBE_ENGINE_TOOLKITGUICONFIG_H
#define RCBE_ENGINE_TOOLKITGUICONFIG_H

#include <string>

#include "json/json.h"

namespace rcbe::config
{
struct ToolkitGUIConfig
{
  std::string widgets_dir_path;
  std::string main_window_widget_file_name;
};

void to_json(nlohmann::json &json, const ToolkitGUIConfig &conf);
void from_json(const nlohmann::json &json, ToolkitGUIConfig &conf);
}

#endif //RCBE_ENGINE_TOOLKITGUICONFIG_H
