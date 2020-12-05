#ifndef RCBE_FILE_UTILS_H
#define RCBE_FILE_UTILS_H

#include <filesystem>
#include <vector>
#include <string>

namespace rcbe::utils
{
// TODO: apparentely current notation is make_path_from_parts
std::filesystem::path makePathFromParts(const std::vector<std::string> &parts);

// TODO: apparentely current notation is make_string_path_from_parts
std::string makeStringPathFromParts(const std::vector<std::string> &parts);

}

#endif //RCBE_ENGINE_FILE_UTILS_H
