#include <fstream>

#include <iostream>

#include <tinyxml2/tinyxml2.h>

#include <rcbe-engine/parsers/x3d/x3d_parser.hpp>

namespace {

inline constexpr const char *SHAPE_NODE_NAME = "Shape";

std::vector<rcbe::math::Vector3d> parse_vertices(std::string &&str) {
    std::vector<rcbe::math::Vector3d> ret;

    std::istringstream iss {str};

    while (!iss.eof()) {
        rcbe::geometry::Mesh::VertexType v;
        iss >> v;
        ret.push_back(v);
    }
    return ret;
}

std::vector<rcbe::math::Vector3d> parse_normals(std::string &&str) {
    std::vector<rcbe::math::Vector3d> ret;

    std::istringstream iss {str};

    while (!iss.eof()) {
        rcbe::geometry::Mesh::NormalType n;
        iss >> n;
        ret.push_back(n);
    }

    return ret;
}

std::vector<rcbe::geometry::Mesh::TriangleType> parse_facets(std::string &&str) {
    std::vector<rcbe::geometry::Mesh::TriangleType> ret;

    std::istringstream iss {str};

    while (!iss.eof()) {
        rcbe::geometry::triangle_indices facet;
        iss >> facet;
        ret.push_back(facet);
    }

    return ret;
}

rcbe::geometry::Mesh::TexCoordStorage parse_tex_coord(std::string &&str) {
    rcbe::geometry::Mesh::TexCoordStorage ret;

    std::istringstream iss {str};

    while (!iss.eof()) {
        rcbe::geometry::Mesh::TexCoordType tex_coord;
        iss >> tex_coord;
        ret.push_back(tex_coord);
    }

    return ret;
}

void parse_tex_coord_index(std::string &&str, rcbe::geometry::Mesh::FacetStorage &facets) {
    std::istringstream iss {str};

    size_t index = 0;

    for (auto &f : facets) {
        size_t sep;
        iss >> f.tex_coords_indices[0] >> f.tex_coords_indices[1] >> f.tex_coords_indices[2] >> sep;
    }
}

std::vector<rcbe::geometry::Mesh::TriangleType> parse_facets(const tinyxml2::XMLElement *element) {
    auto cind_attr = element->Attribute("coordIndex");
    if (!cind_attr)
        return {};

    auto facets = parse_facets(std::string(cind_attr));

    auto tex_coords_elem = element->ToElement()->Attribute("texCoordIndex");
    if (tex_coords_elem) {
        parse_tex_coord_index(std::string(tex_coords_elem), facets);
    }

    return facets;
}

std::vector<rcbe::math::Vector3d> parse_vertices(const tinyxml2::XMLElement *element) {
    auto coord_elem = element->FirstChildElement("Coordinate");
    if (!coord_elem)
        return {};

    auto coord_point_elem = coord_elem->ToElement()->Attribute("point");
    if (!coord_point_elem)
        return {};

    return parse_vertices(std::string(coord_point_elem));
}

std::vector<rcbe::math::Vector3d> parse_normals(const tinyxml2::XMLElement *element) {
    auto normal_elem = element->FirstChildElement("Normal");
    if (!normal_elem)
        return {};

    auto vector_elem = normal_elem->ToElement()->Attribute("vector");
    if (!vector_elem)
        return {};
    
    return parse_normals(std::string(vector_elem));
}

rcbe::geometry::Mesh parse_shape(const tinyxml2::XMLElement *shape_node) {
    auto ifs_elem = shape_node->FirstChildElement("IndexedFaceSet")->ToElement();
    if (!ifs_elem)
        return {};
        
    auto facets = parse_facets(ifs_elem);
    auto vertices = parse_vertices(ifs_elem);
    auto normals = parse_normals(ifs_elem);

    if (facets.empty() || vertices.empty() || normals.empty())
        throw std::runtime_error("Can't parse Mesh data!");
    
    auto texcoord_elem = ifs_elem->FirstChildElement("TextureCoordinate");
    if (!texcoord_elem) 
        return {rcbe::geometry::Mesh(
            vertices.begin(), vertices.end(),
            normals.begin(), normals.end(),
            facets.begin(), facets.end())};

    
    auto tc_point_elem = texcoord_elem->ToElement()->Attribute("point");
    if (!tc_point_elem)
        return {};

    auto tex_coord = parse_tex_coord(std::string(tc_point_elem));

    return rcbe::geometry::Mesh(
        vertices.begin(), vertices.end(),
        normals.begin(), normals.end(),
        facets.begin(), facets.end(),
        tex_coord.begin(), tex_coord.end());
}


}// namespace

namespace rcbe::parsers {

std::vector<geometry::Mesh> parse_meshes(const std::string &file_path) {
    std::vector<geometry::Mesh> ret;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(file_path.c_str());
    tinyxml2::XMLHandle doc_handle(std::addressof(doc));
    auto *scene = doc_handle.FirstChildElement("X3D").FirstChildElement("Scene").ToElement();
    if (!scene) 
        return {};

    for (auto child = scene->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        if (std::string(child->Value()) == std::string(SHAPE_NODE_NAME)) {
            ret.push_back(parse_shape(child));
        }
    }

    return ret;
}

}// namespace rcbe::parsers