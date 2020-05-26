#include <gtest/gtest.h>

#include <iostream>

#include <data_types/math/Matrix.hpp>
#include <common/fuzzy_logic/fuzzy_logic.hpp>

TEST(MatrixTest, IdentityTest)
{
    rcbe::math::Matrix3x3 m{};

    ASSERT_EQ(m.at(0, 0), 1);
    ASSERT_EQ(m.at(1, 1), 1);
    ASSERT_EQ(m.at(2, 2), 1);
}

TEST(MatrixTest, MatrixMultiplication)
{
    rcbe::math::Matrix3x3 m1(
        4, 16, 2,
        7, 12, 11,
        3, 1, 0
    );

    rcbe::math::Matrix3x3 m2(
        17, 1, 22,
        35, 6, 18,
        7, 4, 32
    );

    auto m3 = m1 * m2;

    ASSERT_EQ(m3.at(0, 0), 642);
    ASSERT_EQ(m3.at(0, 1), 108);
    ASSERT_EQ(m3.at(0, 2), 440);
    ASSERT_EQ(m3.at(1, 0), 616);
    ASSERT_EQ(m3.at(1, 1), 123);
    ASSERT_EQ(m3.at(1, 2), 722);
    ASSERT_EQ(m3.at(2, 0), 86);
    ASSERT_EQ(m3.at(2, 1), 9);
    ASSERT_EQ(m3.at(2, 2), 84);
}

TEST(MatrixTest, MatrixAddition)
{
    rcbe::math::Matrix3x3 m1(
        4, 16, 2,
        7, 12, 11,
        3, 1, 0
    );

    rcbe::math::Matrix3x3 m2(
        17, 1, 22,
        35, 6, 18,
        7, 4, 32
    );

    auto m3 = m1 + m2;

    ASSERT_EQ(m3.at(0, 0), 21);
    ASSERT_EQ(m3.at(0, 1), 17);
    ASSERT_EQ(m3.at(0, 2), 24);
    ASSERT_EQ(m3.at(1, 0), 42);
    ASSERT_EQ(m3.at(1, 1), 18);
    ASSERT_EQ(m3.at(1, 2), 29);
    ASSERT_EQ(m3.at(2, 0), 10);
    ASSERT_EQ(m3.at(2, 1), 5);
    ASSERT_EQ(m3.at(2, 2), 32);
}

TEST(MatrixTest, MatrixTransposition)
{
    rcbe::math::Matrix3x3 m1(
        4, 16, 2,
        7, 12, 11,
        3, 1, 0
    );

    auto tm = m1.transpose();

    ASSERT_EQ(tm.at(0, 0), m1.at(0, 0));
    ASSERT_EQ(tm.at(0, 1), m1.at(1, 0));
    ASSERT_EQ(tm.at(0, 2), m1.at(2, 0));
    ASSERT_EQ(tm.at(1, 0), m1.at(0, 1));
    ASSERT_EQ(tm.at(1, 1), m1.at(1, 1));
    ASSERT_EQ(tm.at(1, 2), m1.at(2, 1));
    ASSERT_EQ(tm.at(2, 0), m1.at(0, 2));
    ASSERT_EQ(tm.at(2, 1), m1.at(1, 2));
    ASSERT_EQ(tm.at(2, 2), m1.at(2, 2));
}

TEST(MatrixVectorTest, MatrixVectorMultiplication)
{
    rcbe::math::Matrix3x3 m1(
        2, 4, 0,
        -2, 1, 3,
        -1, 0, 1
    );

    rcbe::math::Vector3d v(1, 2, -1);

    auto v2 = m1 * v;

    ASSERT_EQ(v2.x(), 10);
    ASSERT_EQ(v2.y(), -3);
    ASSERT_EQ(v2.z(), -2);
}

TEST(MatrixQuaternion, CreateMatrixFromQuat)
{
    rcbe::math::Quaternion<rcbe::core::EngineScalar> q { 0, 0, 90 };
    rcbe::math::Matrix3x3 m(q);
 
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(0, 0), 1.0));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(0, 1), 0.0));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(0, 2), 0.0));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(1, 0), 0.0));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(1, 1), 0.0));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(1, 2), -1.0));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(2, 0), 0.0));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(2, 1), 1.0));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(m.at(2, 2), 0.0));
}
