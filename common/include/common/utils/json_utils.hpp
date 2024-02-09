#ifndef RCBE_ENGINE_JSON_UTILS_HPP
#define RCBE_ENGINE_JSON_UTILS_HPP

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

namespace rcbe::utils {

nlohmann::json read_raw(const std::string &path);

template <typename ReturnConfig>
ReturnConfig read_from_file(const std::string &path) {
    std::ifstream ifs(path);

    if (!ifs)
        throw std::runtime_error("File " + path + " doesn't exist or malfromed!");

    nlohmann::json j;
    ifs >> j;

    ReturnConfig ret = j;

    return ret;
}

}// namespace rcbe::utils

#endif//RCBE_ENGINE_JSON_UTILS_HPP
