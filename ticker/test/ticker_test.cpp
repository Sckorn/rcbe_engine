#include <iostream>

#include <gtest/gtest.h>

#include <ticker/Ticker.hpp>

/// TODO: B:8693hbbcg fixme @sckorn @radameon
/*
TEST(TickerTests, FourInvocationsSimpleTests) {
    size_t counter = 0;
    rcbe::core::Ticker tick (std::chrono::milliseconds (30), [&counter]() mutable {
        ++counter;
    } );

    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    tick.stop();

    ASSERT_EQ(counter, 4);
}
*/

TEST(TickerTests, TenInvocationsSimpleTest) {
    size_t counter = 0;
    rcbe::core::Ticker tick(std::chrono::seconds(1), [&counter]() mutable {
        ++counter;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    tick.stop();

    ASSERT_EQ(counter, 10);
}