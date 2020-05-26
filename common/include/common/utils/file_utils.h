#ifndef RCBE_FILE_UTILS_H
#define RCBE_FILE_UTILS_H

#include <filesystem>
#include <vector>
#include <string>

namespace rcbe::utils
{
std::filesystem::path makePathFromParts(const std::vector<std::string> &parts);

std::string makeStringPathFromParts(const std::vector<std::string> &parts);

}

#endif //RCBE_ENGINE_FILE_UTILS_H
