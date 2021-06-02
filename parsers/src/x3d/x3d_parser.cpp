#include <rcbe-engine/parsers/x3d/x3d_parser.hpp>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace
{
namespace pt = boost::property_tree;

static constexpr const char * SHAPE_NODE_NAME = "Shape";

std::vector<rcbe::math::Vector3d> parse_vertices(std::string &&str) {
    std::vector<rcbe::math::Vector3d> ret;

    std::istringstream iss { str };

    while (!iss.eof()) {
        rcbe::geometry::Mesh::VertexType v;
        iss >> v;
        ret.push_back(v);
    }
    return ret;
}

std::vector<rcbe::math::Vector3d> parse_normals(std::string &&str) {
    std::vector<rcbe::math::Vector3d> ret;

    std::istringstream iss { str };

    while (!iss.eof()) {
        rcbe::geometry::Mesh::NormalType n;
        iss >> n;
        ret.push_back(n);
    }

    return ret;
}

std::vector<rcbe::geometry::Mesh::TriangleType> parse_facets(std::string &&str) {
    std::vector<rcbe::geometry::Mesh::TriangleType> ret;

    std::istringstream iss { str };

    while (!iss.eof()) {
        rcbe::geometry::triangle_indices facet;
        iss >> facet;
        ret.push_back(facet);
    }

    return ret;
}

rcbe::geometry::Mesh::TexCoordStorage parse_tex_coord(std::string &&str) {
    rcbe::geometry::Mesh::TexCoordStorage ret;

    std::istringstream iss { str };

    while (!iss.eof()) {
        rcbe::geometry::Mesh::TexCoordType  tex_coord;
        iss >> tex_coord;
        ret.push_back(tex_coord);
    }

    return ret;
}

void parse_tex_coord_index(std::string &&str, rcbe::geometry::Mesh::FacetStorage &facets) {
    std::istringstream iss { str };

    size_t index = 0;

    for (auto &f : facets) {
        size_t sep;
        iss >> f.tex_coords_indices[0] >> f.tex_coords_indices[1] >> f.tex_coords_indices[2] >> sep;
    }
}

rcbe::geometry::Mesh parse_shape(const pt::ptree &subtree)
{
    auto vertices = parse_vertices(subtree.get<std::string>("IndexedFaceSet.Coordinate.<xmlattr>.point"));
    auto normals = parse_normals(subtree.get<std::string>("IndexedFaceSet.Normal.<xmlattr>.vector"));
    auto facets = parse_facets(subtree.get<std::string>("IndexedFaceSet.<xmlattr>.coordIndex"));

    auto optional_tex_coord = subtree.get_optional<std::string>("IndexedFaceSet.TextureCoordinate.<xmlattr>.point");

    if (!optional_tex_coord)
        return rcbe::geometry::Mesh(
                vertices.begin(), vertices.end(),
                normals.begin(), normals.end(),
                facets.begin(), facets.end());

    auto tex_coord = parse_tex_coord(std::move(*optional_tex_coord));

    parse_tex_coord_index(subtree.get<std::string>("IndexedFaceSet.<xmlattr>.texCoordIndex"), facets);

    return rcbe::geometry::Mesh(
            vertices.begin(), vertices.end(),
            normals.begin(), normals.end(),
            facets.begin(), facets.end(),
            tex_coord.begin(), tex_coord.end()
            );
}
}

namespace rcbe::parsers
{
std::vector<geometry::Mesh> parse_meshes(const std::string &file_path)
{
    return parse_meshes(std::ifstream{file_path});
}

std::vector<geometry::Mesh> parse_meshes(std::istream &&input_stream)
{
    pt::ptree pt;
    pt::read_xml(input_stream, pt);

    std::vector<geometry::Mesh> ret;
    for (const auto &[node_name, subtree] : pt.get_child("X3D.Scene"))
    {
        if (std::string(SHAPE_NODE_NAME) == node_name)
        {
            ret.push_back(parse_shape(subtree));
        }
    }

    return ret;
}
}