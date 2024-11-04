#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/math/Quaternion.hpp>
#include <rcbe-engine/datamodel/math/Transform.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/fuzzy_logic/fuzzy_logic.hpp>
#include <rcbe-engine/fundamentals/constants.hpp>

TEST(TransformTests, TranslateVectorPositive) {
    rcbe::math::Vector3d vec {0, 0, 0};

    rcbe::math::Transform t {rcbe::math::Matrix3x3 {}, rcbe::math::Vector3d {2, 1, 0}};

    auto new_vec = t * vec;

    ASSERT_EQ(new_vec.x(), 2);
    ASSERT_EQ(new_vec.y(), 1);
    ASSERT_EQ(new_vec.z(), 0);
}

TEST(TransformTests, TranslateVectorNegative) {
    rcbe::math::Vector3d vec {5, 5, 5};

    rcbe::math::Transform t {rcbe::math::Matrix3x3 {}, rcbe::math::Vector3d {-2, -1, 0}};

    auto new_vec = t * vec;

    ASSERT_EQ(new_vec.x(), 3);
    ASSERT_EQ(new_vec.y(), 4);
    ASSERT_EQ(new_vec.z(), 5);
}

TEST(TransformTests, RotateVectorNinety) {
    rcbe::math::Vector3d vec {0, 0, 1};
    rcbe::math::yaw y(rcbe::math::deg(0));
    rcbe::math::pitch p(rcbe::math::deg(90));
    rcbe::math::roll r(rcbe::math::deg(0));
    rcbe::math::Quaternion<rcbe::core::EngineScalar> q {y, p, r};

    rcbe::math::Transform t {rcbe::math::Matrix3x3 {q}, {0, 0, 0}};

    auto new_vec = t * vec;

    ASSERT_NEAR(new_vec.x(), 1.0, rcbe::core::EPSILON);
}

TEST(TransformTests, RotateVectorFourtyFive) {
    rcbe::math::Vector3d vec {0, 0, 1};
    rcbe::math::yaw y(rcbe::math::deg(0));
    rcbe::math::pitch p(rcbe::math::deg(45));
    rcbe::math::roll r(rcbe::math::deg(0));
    rcbe::math::Quaternion<rcbe::core::EngineScalar> q {y, p, r};

    rcbe::math::Transform t {rcbe::math::Matrix3x3 {q}, {0, 0, 0}};

    auto new_vec = t * vec;

    ASSERT_TRUE(rcbe::core::fuzzy_equal(new_vec.x(), 0.707107));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(new_vec.z(), 0.707107));
}