#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/visual/Texture.hpp>

TEST(Textures, PNG) {
    rcbe::visual::Texture tex {"external/brick_wall_texture/file/brick_wall_texture.tga", rcbe::visual::texture_config {}};
    const auto total_values = tex.getWidth() * tex.getHeight();
    ASSERT_EQ(total_values, tex.getImageBody().size());
}