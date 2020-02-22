#include <gtest/gtest.h>

#include <data_types/math/Transform.hpp>
#include <data_types/math/Vector.hpp>
#include <data_types/math/Quaternion.hpp>
#include <common/fuzzy_logic/fuzzy_logic.hpp>

TEST(TransformTests, TranslateVectorPositive)
{
    rcbe::math::Vector3d vec {0, 0, 0};

    rcbe::math::Transform t {rcbe::math::Matrix3x3 {}, rcbe::math::Vector3d {2, 1, 0}};

    auto new_vec = t * vec;

    ASSERT_EQ(new_vec.x(), 2);
    ASSERT_EQ(new_vec.y(), 1);
    ASSERT_EQ(new_vec.z(), 0);
}

TEST(TransformTests, TranslateVectorNegative)
{
    rcbe::math::Vector3d vec {5, 5, 5};

    rcbe::math::Transform t {rcbe::math::Matrix3x3 {}, rcbe::math::Vector3d {-2, -1, 0}};

    auto new_vec = t * vec;

    ASSERT_EQ(new_vec.x(), 3);
    ASSERT_EQ(new_vec.y(), 4);
    ASSERT_EQ(new_vec.z(), 5);
}

TEST(TransformTests, RotateVectorNinety)
{
    rcbe::math::Vector3d vec {0, 0, 1};
    rcbe::math::Quaternion<rcbe::core::EngineScalar> q {0, 90, 0};

    rcbe::math::Transform t { rcbe::math::Matrix3x3 { q }, { 0, 0, 0 } };

    auto new_vec = t * vec;

    ASSERT_EQ(new_vec.x(), 1);
}

TEST(TransformTests, RotateVectorFourtyFive)
{
    rcbe::math::Vector3d vec {0, 0, 1};
    rcbe::math::Quaternion<rcbe::core::EngineScalar> q {0, 45, 0};

    rcbe::math::Transform t { rcbe::math::Matrix3x3 { q }, { 0, 0, 0 } };

    auto new_vec = t * vec;

    ASSERT_TRUE(rcbe::core::fuzzy_equal(new_vec.x(), 0.707107));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(new_vec.z(), 0.707107));
}