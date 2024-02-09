#include <iostream>

#include "gtest/gtest.h"

#include <rcbe-engine/parsers/x3d/x3d_parser.hpp>

TEST(ParsersTest, TestX3dParse) {
    using namespace rcbe::parsers;
    const auto meshes = parse_meshes("parsers/test/resources/corner.x3d");
    ASSERT_EQ(meshes.size(), 1);
    const auto &mesh = meshes.at(0);
    ASSERT_EQ(mesh.verticesSize(), 12);
    ASSERT_EQ(mesh.normalsSize(), 20);
    ASSERT_EQ(mesh.facetsSize(), 20);
    ASSERT_EQ(mesh.texCoordSize(), 4);

    ASSERT_NE(mesh.facets()[0].tex_coords_indices[0], mesh.facets()[0].tex_coords_indices[1]);
    ASSERT_NE(mesh.facets()[0].tex_coords_indices[1], mesh.facets()[0].tex_coords_indices[2]);
    ASSERT_NE(mesh.facets()[0].tex_coords_indices[2], mesh.facets()[0].tex_coords_indices[0]);
}