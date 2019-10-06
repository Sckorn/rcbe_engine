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

public:
    Mesh() = default;

private:
    std::vector<math::Vector3d> _vertices;
    std::vector<math::Vector3d> _normals;
    std::vector<Triangle> _facets;

};
} // namespace rcbe::geometry


#endif