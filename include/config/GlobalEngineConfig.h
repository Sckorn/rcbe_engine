#ifndef RCBE_ENGINE_GLOBALENGINECONFIG_H
#define RCBE_ENGINE_GLOBALENGINECONFIG_H

#include "json/json.h"

namespace rcbe::config
{
struct GlobalEngineConfig
{
  bool debug = false;
};

void to_json(nlohmann::json &json, const GlobalEngineConfig &config);
void from_json(const nlohmann::json &json, GlobalEngineConfig &config);
}

#endif //RCBE_ENGINE_GLOBALENGINECONFIG_H
