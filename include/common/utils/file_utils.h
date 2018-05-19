#ifndef RCBE_ENGINE_FILE_UTILS_H
#define RCBE_ENGINE_FILE_UTILS_H

#include <boost/filesystem.hpp>

namespace rcbe::common
{
template <typename ... Args>
boost::filesystem::path makePathFromParts(Args&&... args)
{
  return (boost::filesystem::path(args) / ...);
}

template <typename ... Args>
std::string makeStringPathFromParts(Args&&... args)
{
  auto path = makePathFromParts(args...);
  return path.string();
}
}

#endif //RCBE_ENGINE_FILE_UTILS_H
