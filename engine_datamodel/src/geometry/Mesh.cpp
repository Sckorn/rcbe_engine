#include <data_types/geometry/Mesh.hpp>

namespace rcbe::geometry
{
const Mesh::vertex_storage &Mesh::vertices() const
{
    return _vertices;
}

const Mesh::normal_storage &Mesh::normals() const
{
    return _normals;
}

const Mesh::facet_storage &Mesh::facets() const
{
    return _facets;
}

size_t Mesh::vertices_size() const
{
    return _vertices.size();
}

size_t Mesh::normals_size() const
{
    return _normals.size();
}

size_t Mesh::facets_size() const
{
    return _facets.size();
}

Mesh::vertices_iterator Mesh::vertices_begin()
{
    return _vertices.begin();
}

Mesh::const_vertices_iterator Mesh::vertices_cbegin() const
{
    return _vertices.cbegin();
}

Mesh::vertices_iterator Mesh::vertices_end()
{
    return _vertices.end();
}

Mesh::const_vertices_iterator Mesh::vertices_cend() const
{
    return  _vertices.cend();
}

Mesh::normals_iterator Mesh::normals_begin()
{
    return _normals.begin();
}

Mesh::const_normals_iterator Mesh::normals_cbegin() const
{
    return _normals.cbegin();
}

Mesh::normals_iterator Mesh::normals_end()
{
    return _normals.end();
}

Mesh::const_normals_iterator Mesh::normals_cend() const
{
    return _normals.cend();
}

Mesh::facets_iterator Mesh::facets_begin()
{
    return _facets.begin();
}

Mesh::const_facets_iterator Mesh::facets_cbegin() const
{
    return _facets.cbegin();
}

Mesh::facets_iterator Mesh::facets_end()
{
    return _facets.end();
}

Mesh::const_facets_iterator Mesh::facets_cend() const
{
    return _facets.cend();
}

const Mesh::color_type &Mesh::color() const
{
    return _color;
}
} // namespace rcbe::geometry

namespace nlohmann
{
void adl_serializer<rcbe::geometry::Mesh>::to_json(nlohmann::json &j, const rcbe::geometry::Mesh &m)
{
    j = {
        {"vertices", m.vertices()},
        {"normals", m.normals()},
        {"facets", m.facets()},
        {"color", m.color()}
    };
}

void adl_serializer<rcbe::geometry::Mesh>::from_json(const nlohmann::json &j, rcbe::geometry::Mesh &m)
{
    auto vertices = j.at("vertices").get<rcbe::geometry::Mesh::vertex_storage>();
    auto normals = j.at("normals").get<rcbe::geometry::Mesh::normal_storage>();
    auto facets = j.at("facets").get<rcbe::geometry::Mesh::facet_storage>();
    auto color = j.at("color").get<rcbe::geometry::Mesh::color_type>();

    m = { 
        std::make_move_iterator(vertices.begin()), std::make_move_iterator(vertices.end()), 
        std::make_move_iterator(normals.begin()), std::make_move_iterator(normals.end()),
        std::make_move_iterator(facets.begin()), std::make_move_iterator(facets.end()),
        color
    };
}
}
