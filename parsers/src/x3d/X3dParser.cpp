#include <parsers/x3d/X3dParser.hpp>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace
{
namespace pt = boost::property_tree;

static constexpr const char * SHAPE_NODE_NAME = "Shape";

std::vector<rcbe::math::Vector3d> parseVertices(std::string &&str)
{
    std::vector<rcbe::math::Vector3d> ret;

    std::istringstream iss { str };

    while (!iss.eof())
    {
        rcbe::geometry::Mesh::vertex_type v;
        iss >> v;
        ret.push_back(v);
    }
    return ret;
}

std::vector<rcbe::math::Vector3d> parseNormals(std::string &&str)
{
    std::vector<rcbe::math::Vector3d> ret;

    std::istringstream iss { str };

    while (!iss.eof())
    {
        rcbe::geometry::Mesh::normal_type n;
        iss >> n;
        ret.push_back(n);
    }

    return ret;
}

std::vector<rcbe::geometry::Mesh::triangle_type> parseFacets(std::string &&str)
{
    std::vector<rcbe::geometry::Mesh::triangle_type> ret;

    std::istringstream iss { str };

    while (!iss.eof()) 
    {
        rcbe::geometry::Triangle facet;
        iss >> facet;
        ret.push_back(facet);
    }

    return ret;
}

rcbe::geometry::Mesh parseShape(const pt::ptree &subtree)
{
    auto vertices = parseVertices(subtree.get<std::string>("IndexedFaceSet.Coordinate.<xmlattr>.point"));
    auto normals = parseNormals(subtree.get<std::string>("IndexedFaceSet.Normal.<xmlattr>.vector"));
    auto facets = parseFacets(subtree.get<std::string>("IndexedFaceSet.<xmlattr>.coordIndex"));

    return rcbe::geometry::Mesh(vertices.begin(), vertices.end(), normals.begin(), normals.end(), facets.begin(), facets.end());
}
}

namespace rcbe::parsers
{
std::vector<geometry::Mesh> parseMeshes(const std::string &file_path)
{
    return parseMeshes(std::ifstream { file_path } );
}

std::vector<geometry::Mesh> parseMeshes(std::istream &&input_stream)
{
    pt::ptree pt;
    pt::read_xml(input_stream, pt);

    std::vector<geometry::Mesh> ret;
    for (const auto &[node_name, subtree] : pt.get_child("X3D.Scene"))
    {
        if (std::string(SHAPE_NODE_NAME) == node_name)
        {
            ret.push_back(parseShape(subtree));
        }
    }

    return ret;
}
}