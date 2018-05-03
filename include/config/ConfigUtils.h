#ifndef RCBE_ENGINE_CONFIGUTILS_H
#define RCBE_ENGINE_CONFIGUTILS_H

#include <fstream>

#include <boost/filesystem.hpp>

#include "json/json.h"
#include "config/GlobalEngineConfig.h"

namespace rcbe::config::utils
{

template <typename ReturnConfig>
ReturnConfig readFromFile(const std::string &path)
{
  ReturnConfig ret;

  std::ifstream ifs(path);

  if(!ifs)
    throw std::runtime_error("File doesn't exist or malfromed!");

  nlohmann::json j;
  ifs >> j;

  from_json(j, ret);

  return ret;
}

}

#endif //RCBE_ENGINE_CONFIGUTILS_H
