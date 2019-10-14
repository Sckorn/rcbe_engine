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
} // namespace rcbe::geometry
