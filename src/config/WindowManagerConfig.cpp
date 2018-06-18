#include "config/WindowManagerConfig.h"

namespace rcbe::config
{
void to_json(nlohmann::json &json, const WindowData &conf)
{
  nlohmann::json children_array;
  for(const auto &entry : conf.children)
  {
    nlohmann::json tmp;
    to_json(tmp, entry);
    children_array.push_back(tmp);
  }

  json = nlohmann::json{
    {"id", conf.id},
    {"file_name", conf.file_name},
    {"children", children_array}
  };
}

void from_json(const nlohmann::json &json, WindowData &conf)
{
  conf.id = json.at("id").get<std::string>();
  conf.file_name = json.at("file_name").get<std::string>();

  auto children_json = json.at("children");
  for(auto it = children_json.begin(); it != children_json.end(); ++it)
  {
    WindowData tmp;
    from_json(*it, tmp);
    conf.children.push_back(tmp);
  }
}

void to_json(nlohmann::json &json, const WindowManagerConfig &conf)
{
  /*nlohmann::json tmp;
  to_json(tmp, );*/
  json = nlohmann::json{
    {"widgets_dir_path", conf.widgets_dir_path},
    {"widgets", conf.widgets}
  };
}

void from_json(const nlohmann::json &json, WindowManagerConfig &conf)
{
  conf.widgets_dir_path = json.at("widgets_dir_path").get<std::string>();
  conf.widgets = json.at("widgets").get<WindowData>();
}
}