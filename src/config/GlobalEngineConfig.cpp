#include "config/GlobalEngineConfig.h"

namespace rcbe::config
{
void to_json(nlohmann::json &json, const GlobalEngineConfig &config)
{
  json = nlohmann::json{
    {"debug", config.debug},
    {"application_name", config.application_name},
    {"window_manager_config_file", config.window_manager_config_file}
  };
}

void from_json(const nlohmann::json &json, GlobalEngineConfig &config)
{
  config.debug = json.at("debug").get<bool>();
  config.application_name = json.at("application_name").get<std::string>();
  config.window_manager_config_file = json.at("window_manager_config_file").get<std::string>();
}
}