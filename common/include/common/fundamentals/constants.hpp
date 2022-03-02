#ifndef RCBE_FUNDAMENTALS_CONSTANTS_HPP
#define RCBE_FUNDAMENTALS_CONSTANTS_HPP

#include <filesystem>

namespace rcbe::core {
inline constexpr double EPSILON = 0.0001;

#ifdef _WIN32
static const std::filesystem::path TEMP_PATH = "C:\\Windows\\Temp";
#elif defined(__linux__)
static const std::filesystem::path TEMP_PATH = "/tmp/";
#else
static_assert(false, "Only Windows and Linux builds are supported as of now!");
#endif
}

#endif