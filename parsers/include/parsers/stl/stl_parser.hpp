#ifndef RCBE_ENGINE_STL_PARSER_HPP
#define RCBE_ENGINE_STL_PARSER_HPP

#include <filesystem>
#include <vector>

#include <rcbe-engine/datamodel/geometry/Mesh.hpp>

namespace rcbe::parsers::stl {/// TODO: leave this until a better solution is introduced
R_PUBLIC_API geometry::Mesh parse_mesh(const std::filesystem::path &file_path);
}

#endif//RCBE_ENGINE_STL_PARSER_HPP
