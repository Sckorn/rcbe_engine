#ifndef RCBE_ENGINE_GLOBALENGINECONFIG_H
#define RCBE_ENGINE_GLOBALENGINECONFIG_H

#include <nlohmann/json.h>

namespace rcbe::config
{
struct GlobalEngineConfig
{
  bool debug = false;
  std::string application_name;
  std::string window_manager_config_file;
  std::string app_base_path;
};

void to_json(nlohmann::json &json, const GlobalEngineConfig &config);
void from_json(const nlohmann::json &json, GlobalEngineConfig &config);
}

#endif //RCBE_ENGINE_GLOBALENGINECONFIG_H
