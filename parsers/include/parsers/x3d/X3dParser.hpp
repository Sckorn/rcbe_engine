#ifndef RCBE_X3DPARSER_HPP
#define RCBE_X3DPARSER_HPP

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <data_types/geometry/Mesh.hpp>

namespace rcbe::parsers
{
std::vector<geometry::Mesh> parseMeshes(const std::string &file_path);
}

#endif