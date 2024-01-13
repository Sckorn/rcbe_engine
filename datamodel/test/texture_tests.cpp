#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include <rcbe-engine/parsers/tga/tga_parser.hpp>

TEST(Textures, TGA) {
    rcbe::visual::Texture tex("external/brick_wall_texture/file/brick_wall_texture.tga", rdmn::parse::tga::parse);
    const auto total_values = tex.getWidth() * tex.getHeight();
    ASSERT_EQ(total_values, tex.getPixels().size());
}