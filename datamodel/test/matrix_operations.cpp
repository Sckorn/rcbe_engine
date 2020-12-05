#include <gtest/gtest.h>

#include <iostream>

#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/fuzzy_logic/fuzzy_logic.hpp>

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

TEST(MatrixTest, Matrix4by4Multiplication) {
    rcbe::math::Matrix4x4 m1(
            4, 16, 2, 8,
            7, 12, 11, -3,
            3, 1, 0, 5,
            0, 2, 0, 1
    );

    rcbe::math::Matrix4x4 m2(
            12, 3, 16, 0,
            1, 2, 7, -4,
            5, -10, -2, 1,
            3, 14, -6, 0
            );

    auto m3 = m1 * m2;

    ASSERT_EQ(m3.at(0, 0), 98);
    ASSERT_EQ(m3.at(0, 1), 136);
    ASSERT_EQ(m3.at(0, 2), 124);
    ASSERT_EQ(m3.at(0, 3), -62);
    ASSERT_EQ(m3.at(1, 0), 142);
    ASSERT_EQ(m3.at(1, 1), -107);
    ASSERT_EQ(m3.at(1, 2), 192);
    ASSERT_EQ(m3.at(1, 3), -37);
    ASSERT_EQ(m3.at(2, 0), 52);
    ASSERT_EQ(m3.at(2, 1), 81);
    ASSERT_EQ(m3.at(2, 2), 25);
    ASSERT_EQ(m3.at(2, 3), -4);
    ASSERT_EQ(m3.at(3, 0), 5);
    ASSERT_EQ(m3.at(3, 1), 18);
    ASSERT_EQ(m3.at(3, 2), 8);
    ASSERT_EQ(m3.at(3, 3), -8);
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

TEST(MatrixTest, Matrix4by4Addition)
{
    rcbe::math::Matrix4x4 m1(
            4, 16, 2, 1,
            7, 12, 11, -3,
            3, 1, 0, 7,
            15, 20, 6, -3
    );

    rcbe::math::Matrix4x4 m2(
            17, 1, 22, 5,
            35, 6, 18, 4,
            7, 4, 32, -10,
            -10, 5, 4, -2
    );

    auto m3 = m1 + m2;

    ASSERT_EQ(m3.at(0, 0), 21);
    ASSERT_EQ(m3.at(0, 1), 17);
    ASSERT_EQ(m3.at(0, 2), 24);
    ASSERT_EQ(m3.at(0, 3), 6);
    ASSERT_EQ(m3.at(1, 0), 42);
    ASSERT_EQ(m3.at(1, 1), 18);
    ASSERT_EQ(m3.at(1, 2), 29);
    ASSERT_EQ(m3.at(1, 3), 1);
    ASSERT_EQ(m3.at(2, 0), 10);
    ASSERT_EQ(m3.at(2, 1), 5);
    ASSERT_EQ(m3.at(2, 2), 32);
    ASSERT_EQ(m3.at(2, 3), -3);
    ASSERT_EQ(m3.at(3, 0), 5);
    ASSERT_EQ(m3.at(3, 1), 25);
    ASSERT_EQ(m3.at(3, 2), 10);
    ASSERT_EQ(m3.at(3, 3), -5);
}

TEST(MatrixTest, MatrixTransposition)
{
    rcbe::math::Matrix3x3 m1(
        4, 16, 2,
        7, 12, 11,
        3, 1, 0
    );

    auto tm = m1.transposed();

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

TEST(MatrixTest, Matrix4by4Transposition)
{
    rcbe::math::Matrix4x4 m1(
            4, 16, 2, 3,
            7, 12, 11, 1,
            3, 1, 0, 7,
            5, 4, 3, 2
    );

    auto tm = m1.transposed();

    ASSERT_EQ(tm.at(0, 0), m1.at(0, 0));
    ASSERT_EQ(tm.at(0, 1), m1.at(1, 0));
    ASSERT_EQ(tm.at(0, 2), m1.at(2, 0));
    ASSERT_EQ(tm.at(0, 3), m1.at(3, 0));
    ASSERT_EQ(tm.at(1, 0), m1.at(0, 1));
    ASSERT_EQ(tm.at(1, 1), m1.at(1, 1));
    ASSERT_EQ(tm.at(1, 2), m1.at(2, 1));
    ASSERT_EQ(tm.at(1, 3), m1.at(3, 1));
    ASSERT_EQ(tm.at(2, 0), m1.at(0, 2));
    ASSERT_EQ(tm.at(2, 1), m1.at(1, 2));
    ASSERT_EQ(tm.at(2, 2), m1.at(2, 2));
    ASSERT_EQ(tm.at(2, 3), m1.at(3, 2));
    ASSERT_EQ(tm.at(3, 0), m1.at(0, 3));
    ASSERT_EQ(tm.at(3, 1), m1.at(1, 3));
    ASSERT_EQ(tm.at(3, 2), m1.at(2, 3));
    ASSERT_EQ(tm.at(3, 3), m1.at(3, 3));
}

TEST(MatrixTest, Determinant3by3) {
    rcbe::math::Matrix3x3 m1(
            4, 16, 2,
            7, 12, 11,
            3, 1, 0
    );

    ASSERT_EQ(m1.determinant(), 426);
}

TEST(MatrixTest, Determinant4by4) {
    rcbe::math::Matrix4x4 m1(
            4, 16, 2, 8,
            7, 12, 11, -3,
            3, 1, 0, 5,
            0, 2, 0, 1
    );

    ASSERT_EQ(m1.determinant(), 162);
}

TEST(MatrixTest, Inverse3by3) {
    rcbe::math::Matrix3x3 m(
            2, 5, 7,
            6, 3, 4,
            5, -2, -3
            );

    auto inv = m.inversed();

    ASSERT_EQ(inv.at(0, 0), 1);
    ASSERT_EQ(inv.at(0, 1), -1);
    ASSERT_EQ(inv.at(0, 2), 1);
    ASSERT_EQ(inv.at(1, 0), -38);
    ASSERT_EQ(inv.at(1, 1), 41);
    ASSERT_EQ(inv.at(1, 2), -34);
    ASSERT_EQ(inv.at(2, 0), 27);
    ASSERT_EQ(inv.at(2, 1), -29);
    ASSERT_EQ(inv.at(2, 2), 24);

    auto identity = m * inv;

    ASSERT_EQ(identity.at(0, 0), 1);
    ASSERT_EQ(identity.at(0, 1), 0);
    ASSERT_EQ(identity.at(0, 2), 0);
    ASSERT_EQ(identity.at(1, 0), 0);
    ASSERT_EQ(identity.at(1, 1), 1);
    ASSERT_EQ(identity.at(1, 2), 0);
    ASSERT_EQ(identity.at(2, 0), 0);
    ASSERT_EQ(identity.at(2, 1), 0);
    ASSERT_EQ(identity.at(2, 2), 1);
}

TEST(MatrixTest, Inverse4by4) {
    rcbe::math::Matrix4x4 m(
            4, 16, 2, 8,
            7, 12, 11, -3,
            3, 1, 0, 5,
            0, 2, 0, 1
    );

    auto inv = m.inversed();
    auto identity = m * inv;

    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(0, 0), 1.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(0, 1), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(0, 2), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(0, 3), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(1, 0), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(1, 1), 1.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(1, 2), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(1, 3), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(2, 0), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(2, 1), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(2, 2), 1.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(2, 3), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(3, 0), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(3, 1), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(3, 2), 0.));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(identity.at(3, 3), 1.));
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

TEST(MatrixVectorTest, MatrixVectorMultiplication4by4)
{
    rcbe::math::Matrix4x4 m(
            2, 3, -4, 12,
            11, 8, 7, 3,
            2, 5, 3, 5,
            1, 7, -2, 0
    );

    rcbe::math::Vector4d v(3, 7, 5, 1);

    auto v2 = m * v;

    ASSERT_EQ(v2.x(), 19);
    ASSERT_EQ(v2.y(), 127);
    ASSERT_EQ(v2.z(), 61);
    ASSERT_EQ(v2.w(), 42);
}

TEST(MatrixQuaternion, CreateMatrixFromQuat)
{
    rcbe::math::yaw y(rcbe::math::deg(0));
    rcbe::math::pitch p(rcbe::math::deg(0));
    rcbe::math::roll r(rcbe::math::deg(90));
    rcbe::math::Quaternion<rcbe::core::EngineScalar> q { y, p, r };
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
