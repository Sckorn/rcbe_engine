#ifndef RCBE_FILE_UTILS_HPP
#define RCBE_FILE_UTILS_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace rcbe::utils {
std::filesystem::path make_path_from_parts(const std::vector<std::string> &parts);

std::string make_string_path_from_parts(const std::vector<std::string> &parts);

}// namespace rcbe::utils

#endif//RCBE_ENGINE_FILE_UTILS_H
