#include "config/GlobalEngineConfig.h"

namespace rcbe::config
{
void to_json(nlohmann::json &json, const GlobalEngineConfig &config)
{
  json = nlohmann::json{
    {"debug", config.debug}
  };
}

void from_json(const nlohmann::json &json, GlobalEngineConfig &config)
{
  config.debug = json.at("debug").get<bool>();
}
}