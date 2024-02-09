#ifndef RCBE_X3DPARSER_HPP
#define RCBE_X3DPARSER_HPP

#include <rcbe-engine/datamodel/geometry/Mesh.hpp>

namespace rcbe::parsers {
std::vector<geometry::Mesh> parse_meshes(const std::string &file_path);
std::vector<geometry::Mesh> parse_meshes(std::istream &&input_stream);
}// namespace rcbe::parsers

#endif