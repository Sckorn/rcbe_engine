#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/math/Quaternion.hpp>
#include <rcbe-engine/datamodel/math/euler_angles.hpp>
#include <rcbe-engine/fuzzy_logic/fuzzy_logic.hpp>

TEST(QuaternionTest, QuaternionFromRPY) {
    rcbe::math::yaw y(rcbe::math::deg(90));
    rcbe::math::pitch p(rcbe::math::deg(45));
    rcbe::math::roll r(rcbe::math::deg(15));
    rcbe::math::Quaternion<rcbe::core::EngineScalar> q {y, p, r};

    ASSERT_TRUE(rcbe::core::fuzzy_equal(q.x(), -0.183013));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(q.y(), 0.3535534));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(q.z(), 0.6123724));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(q.w(), 0.6830127));
}
