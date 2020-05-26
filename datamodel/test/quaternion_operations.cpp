#include <gtest/gtest.h>

#include <data_types/math/Quaternion.hpp>

#include <common/fuzzy_logic/fuzzy_logic.hpp>

TEST(QuaternionTest, QuaternionFromRPY)
{
    rcbe::math::Quaternion<rcbe::core::EngineScalar> q { 90, 45, 15 };
    
    ASSERT_TRUE(rcbe::core::fuzzy_equal(q.x(), -0.183013));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(q.y(), 0.3535534));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(q.z(), 0.6123724));
    ASSERT_TRUE(rcbe::core::fuzzy_equal(q.w(), 0.6830127));
    
}
