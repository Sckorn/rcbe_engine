#ifndef RCBE_MESH_HPP
#define RCBE_MESH_HPP

#include <vector>

#include <data_types/math/math.hpp>
#include <data_types/geometry/geometry.hpp>

namespace rcbe::geometry
{
class Mesh
{
public:
    using vertex_type = math::Vector3d;
    using normal_type = math::Vector3d;
    using triangle_type = Triangle;

    using vertex_storage = std::vector<math::Vector3d>;
    using normal_storage = std::vector<math::Vector3d>;
    using facet_storage = std::vector<Triangle>;

    using vertices_iterator = vertex_storage::iterator;
    using normals_iterator = normal_storage::iterator;
    using facets_iterator = facet_storage::iterator;

    using const_vertices_iterator = vertex_storage::const_iterator;
    using const_normals_iterator = normal_storage::const_iterator;
    using const_facets_iterator = facet_storage::const_iterator;

public:
    Mesh() = default;

    template <typename VertsInIter, typename NormsInIter, typename FacesInIter>
    Mesh(
        VertsInIter vbegin, VertsInIter vend,
        NormsInIter nbegin, NormsInIter nend,
        FacesInIter fbegin, FacesInIter fend
    )
    :
    _vertices(vbegin, vend)
    , _normals(nbegin, nend)
    , _facets(fbegin, fend)
    {}

    Mesh(const Mesh &other) = delete;
    Mesh &operator=(const Mesh &other) = delete;

    Mesh(Mesh &&other) = default;
    Mesh &operator=(Mesh &&other) = default;

    const vertex_storage &vertices() const;
    const normal_storage &normals() const;
    const facet_storage &facets() const;

    size_t vertices_size() const;
    size_t normals_size() const;
    size_t facets_size() const;

    vertices_iterator vertices_begin();
    const_vertices_iterator vertices_cbegin() const;

    vertices_iterator vertices_end();
    const_vertices_iterator vertices_cend() const;

    normals_iterator normals_begin();
    const_normals_iterator normals_cbegin() const;

    normals_iterator normals_end();
    const_normals_iterator normals_cend() const;

    facets_iterator facets_begin();
    const_facets_iterator facets_cbegin() const;

    facets_iterator facets_end();
    const_facets_iterator facets_cend() const;

private:
    vertex_storage _vertices;
    normal_storage _normals;
    facet_storage _facets;

};
} // namespace rcbe::geometry


#endif