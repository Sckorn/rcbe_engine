#include <fstream>
#include <iostream>

#include <rcbe-engine/binary/BinaryBuffer.hpp>
#include <rcbe-engine/datamodel/geometry/BinaryStlFile.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/parsers/stl/stl_parser.hpp>

namespace rcbe::parsers::stl {
geometry::Mesh parse_mesh(const std::filesystem::path &file_path) {
    std::ifstream ifs {file_path, std::ios::in | std::ios::binary};
    rcbe::binary::BinaryBuffer bb;
    ifs >> bb;

    rcbe::geometry::binary_stl_header header;
    rcbe::binary::from_binary(bb, header);

    geometry::Mesh::FacetStorage facets {};
    geometry::Mesh::VertexStorage verts {};
    geometry::Mesh::NormalStorage norms {};
    geometry::Mesh::TexCoordStorage tex_coords {};

    auto offset = header.SIZE;
    for (size_t i = 0; i < header.number_triangles; ++i) {
        rcbe::geometry::binary_stl_chunk facet;
        auto bbb = bb.at(offset + facet.SIZE * i, facet.SIZE);
        rcbe::binary::from_binary(bbb, facet);

        math::Vector3d norm(facet.normal);

        for (size_t i = 0; i < 3; ++i) {
            norms.push_back(norm);
        }

        verts.emplace_back(facet.v1);
        size_t v1_index = verts.size() - 1;

        verts.emplace_back(facet.v2);
        size_t v2_index = verts.size() - 1;

        verts.emplace_back(facet.v3);
        size_t v3_index = verts.size() - 1;

        facets.emplace_back(v1_index, v2_index, v3_index);

        /// no tex coords in STL, so leaving zeroes for consistency
        tex_coords.emplace_back(0.0, 0.0);
    }

    return geometry::Mesh(
        std::move(verts),
        std::move(norms),
        std::move(facets),
        std::move(tex_coords),
        geometry::Mesh::ColorType(0.5, 0.5, 0.5));
}
}// namespace rcbe::parsers::stl