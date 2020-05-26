#ifndef RCBE_ENGINE_JSON_UTILS_HPP
#define RCBE_ENGINE_JSON_UTILS_HPP

#include <fstream>
#include <nlohmann/json.hpp>

namespace rcbe::utils {

template <typename ReturnConfig>
ReturnConfig readFromFile(const std::string &path)
{
    std::ifstream ifs(path);

    if(!ifs)
        throw std::runtime_error("File doesn't exist or malfromed!");

    nlohmann::json j;
    ifs >> j;

    ReturnConfig ret = j;

    return ret;
}

}

#endif //RCBE_ENGINE_JSON_UTILS_HPP
