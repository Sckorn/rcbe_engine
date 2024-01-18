#include <gtest/gtest.h>

#include <rcbe-engine/datamodel/math/euler_angles.hpp>
#include <rcbe-engine/datamodel/math/rotation_units.hpp>

TEST(AngleTests, ComparisonsTestDegDeg) {
    rcbe::math::deg d1(90);
    rcbe::math::deg d2(45);

    ASSERT_GT(d1, d2);
}

TEST(AngleTests, ComparisonTestRadRad) {
    rcbe::math::rad d1(M_PI);
    rcbe::math::rad d2(M_PI_4);

    ASSERT_GT(d1, d2);
}

TEST(AngleTests, ComparisonMixedUnitsYaw) {
    rcbe::math::yaw r1(rcbe::math::deg(90));
    rcbe::math::yaw r2(rcbe::math::rad(M_PI_4));

    ASSERT_GT(r1, r2);
}

TEST(AngleTests, ComparisonMixedUnitsPitch) {
    rcbe::math::pitch r1(rcbe::math::deg(90));
    rcbe::math::pitch r2(rcbe::math::rad(M_PI_4));

    ASSERT_GT(r1, r2);
}

TEST(AngleTests, AdditionsYaw) {
    rcbe::math::yaw y(rcbe::math::deg(90));
    y += rcbe::math::yaw(rcbe::math::rad(M_PI_4));


    auto double_rep = static_cast<double>(y.as_deg());
    ASSERT_EQ(double_rep, 135);
}

TEST(AngleTests, AdditionsPitch) {
    rcbe::math::pitch p(rcbe::math::deg(90));
    p += rcbe::math::pitch(rcbe::math::rad(M_PI_4));

    auto double_rep = static_cast<double>(p.as_deg());
    ASSERT_EQ(double_rep, 135);
}

TEST(AngleTests, SubtractDeg) {
    rcbe::math::deg ld(90.0);
    rcbe::math::deg rd(30.0);

    auto double_rep = static_cast<double>(ld - rd);
    ASSERT_EQ(double_rep, 60);
}

TEST(AngleTests, SubtractRad) {
    rcbe::math::rad lr(M_PI * 2);
    rcbe::math::rad rr(M_PI);

    auto double_rep = static_cast<double>(lr - rr);
    ASSERT_EQ(double_rep, M_PI);
}