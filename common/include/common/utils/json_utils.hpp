#ifndef RCBE_ENGINE_JSON_UTILS_HPP
#define RCBE_ENGINE_JSON_UTILS_HPP

#include <fstream>
#include <iostream>
#include <iomanip>

#include <nlohmann/json.hpp>

namespace rcbe::utils {

R_PUBLIC_API nlohmann::json read_raw(const std::string &path);

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

template <typename StructType>
bool write_to_file(StructType &&s, std::string &&file_path) {
    std::ofstream ofs {file_path};

    if (!ofs)
        return false;

    nlohmann::json j = s;

    ofs << std::setw(4) << j << std::endl;
    return true;
}

}// namespace rcbe::utils

#endif//RCBE_ENGINE_JSON_UTILS_HPP
