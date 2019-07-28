#include <common/utils/file_utils.h>

namespace rcbe::common
{
boost::filesystem::path makePathFromParts(const std::vector<std::string> &parts)
{
  boost::filesystem::path ret;
  for(const auto &part: parts)
  {
    ret /= boost::filesystem::path(part);
  }
  return ret;
}

std::string makeStringPathFromParts(const std::vector<std::string> &parts)
{
  return makePathFromParts(parts).string();
}
}