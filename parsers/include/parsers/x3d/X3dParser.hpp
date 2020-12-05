#ifndef RCBE_X3DPARSER_HPP
#define RCBE_X3DPARSER_HPP

#include <data_types/geometry/Mesh.hpp>

namespace rcbe::parsers
{
//TODO: wrong notation, refer to code style
std::vector<geometry::Mesh> parseMeshes(const std::string &file_path);
std::vector<geometry::Mesh> parseMeshes(std::istream &&input_stream);
}

#endif