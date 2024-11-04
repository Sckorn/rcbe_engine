#ifndef RCBE_FUNDAMENTALS_TYPES_HPP
#define RCBE_FUNDAMENTALS_TYPES_HPP

#include <filesystem>

#ifdef _WIN32
using ssize_t = ptrdiff_t;
#endif

namespace rcbe::core {
using EngineScalar = double;
using EngineIntergral = int;
// TODO: rename to EngineFsPath, consider moving to a separate header.
using EnginePath = std::filesystem::path;
}// namespace rcbe::core

#endif