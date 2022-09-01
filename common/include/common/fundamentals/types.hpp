#ifndef RCBE_FUNDAMENTALS_TYPES_HPP
#define RCBE_FUNDAMENTALS_TYPES_HPP

#include <filesystem>

namespace rcbe::core {
using EngineScalar = double;
using EngineIntergral = int;
// TODO: rename to EngineFsPath, consider moving to a separate header.
using EnginePath = std::filesystem::path;
}

#endif