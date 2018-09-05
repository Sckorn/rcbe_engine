#ifndef RCBE_ENGINE_FILE_UTILS_H
#define RCBE_ENGINE_FILE_UTILS_H

#include <boost/filesystem.hpp>

namespace rcbe::common
{
boost::filesystem::path makePathFromParts(const std::vector<std::string> &parts);

std::string makeStringPathFromParts(const std::vector<std::string> &parts);
}

#endif //RCBE_ENGINE_FILE_UTILS_H
