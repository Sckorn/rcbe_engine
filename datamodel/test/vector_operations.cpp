#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/math/Vector.hpp>

TEST(VectorTest, VectorAddition)
{
    rcbe::math::Vector3d v1(1, 2, 3);
    rcbe::math::Vector3d v2(5, 7, 2);

    auto v3 = v1 + v2;

    ASSERT_EQ(v3.at(0), 6);
    ASSERT_EQ(v3.at(1), 9);
    ASSERT_EQ(v3.at(2), 5);
}

TEST(VectorTest, VectorSubtraction)
{
    rcbe::math::Vector3d v1(1, 2, 3);
    rcbe::math::Vector3d v2(5, 7, 2);

    auto v3 = v1 - v2;

    ASSERT_EQ(v3.at(0), -4);
    ASSERT_EQ(v3.at(1), -5);
    ASSERT_EQ(v3.at(2), 1);
}

TEST(VectorTest, VectorScalarMultiplication)
{
    rcbe::math::Vector3d v(2, 0, 0);
    ASSERT_EQ(v.length(), 2);

    auto normalized = v.normalized();
    ASSERT_EQ(normalized.length(), 1);

    auto mul = normalized * 3;
    ASSERT_EQ(mul.length(), 3);
}

TEST(VectorTest, CrossProductTest)
{
    rcbe::math::Vector3d v1 { 1, 0, 0 };
    rcbe::math::Vector3d v2 { 0, 1, 0 };

    auto cross = rcbe::math::Vector3d::cross(v1, v2);

    ASSERT_EQ(cross.length(), 1);
    ASSERT_EQ(cross.z(), 1);
}

TEST(VectorTest, CtorsTests)
{
    rcbe::math::Vector3d v3 {1, 2 ,3};
    rcbe::math::Vector4d v4 {4, 5, 6, 7};

    ASSERT_EQ(v3.x(), 1);

    ASSERT_EQ(v4.x(), 4);
}
