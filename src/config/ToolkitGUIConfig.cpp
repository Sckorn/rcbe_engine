#include "config/ToolkitGUIConfig.h"

namespace rcbe::config
{
void to_json(nlohmann::json &json, const ToolkitGUIConfig &conf)
{
  json = nlohmann::json{
    {"widgets_dir_path", conf.widgets_dir_path},
    {"main_window_widget_file_name", conf.main_window_widget_file_name}
  };
}

void from_json(const nlohmann::json &json, ToolkitGUIConfig &conf)
{
  conf.widgets_dir_path = json.at("widgets_dir_path").get<std::string>();
  conf.main_window_widget_file_name = json.at("main_window_widget_file_name").get<std::string>();
}
}