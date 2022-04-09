#include <gtest/gtest.h>

#include <rcbe-engine/parsers/tga/tga_parser.hpp>

TEST(Textures, TGA) {
    const auto [metadata, pixels] = rdmn::parse::tga::parse("external/brick_wall_texture/file/brick_wall_texture.tga");
    const auto total_values = metadata.dimensions.width * metadata.dimensions.height;
    ASSERT_EQ(total_values, pixels->size());
}