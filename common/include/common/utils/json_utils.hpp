#ifndef RCBE_ENGINE_JSON_UTILS_HPP
#define RCBE_ENGINE_JSON_UTILS_HPP

#include <fstream>
#include <nlohmann/json.hpp>

namespace rcbe::utils {

nlohmann::json read_raw(const std::string &path);

template <typename ReturnConfig>
// TODO: apparentely the right notation is now read_from_file
ReturnConfig readFromFile(const std::string &path)
{
    std::ifstream ifs(path);

    if(!ifs)
        // TODO: add file path to output
        throw std::runtime_error("File doesn't exist or malfromed!");

    nlohmann::json j;
    ifs >> j;

    ReturnConfig ret = j;

    return ret;
}

}

#endif //RCBE_ENGINE_JSON_UTILS_HPP
