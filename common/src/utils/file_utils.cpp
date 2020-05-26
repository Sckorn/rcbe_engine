#include <common/utils/file_utils.h>

namespace rcbe::utils
{
std::filesystem::path makePathFromParts(const std::vector<std::string> &parts)
{
  std::filesystem::path ret;
  for(const auto &part: parts)
  {
    ret /= std::filesystem::path(part);
  }
  return ret;
}

std::string makeStringPathFromParts(const std::vector<std::string> &parts)
{
  return makePathFromParts(parts).string();
}
}