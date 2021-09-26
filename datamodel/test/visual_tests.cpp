#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

TEST(ColorsTests, HexCtor) {
    rcbe::visual::RGBAColor yellow(0xFFFF00FF);
    ASSERT_EQ(yellow.r(), 1.0);
    ASSERT_EQ(yellow.g(), 1.0);
    ASSERT_EQ(yellow.b(), 0);
    ASSERT_EQ(yellow.a(), 1.0);
}

TEST(ColorsTest, RedToInt) {
    const uint32_t red_as_int = 0xFF0000FF;
    const rcbe::visual::RGBAColor red(1.0, 0.0, 0.0, 1.0);

    ASSERT_EQ(red_as_int, static_cast<uint32_t>(red));
}