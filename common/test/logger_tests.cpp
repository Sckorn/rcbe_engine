#include <gtest/gtest.h>

#include <rdmn-engine/logger/trivial_logger.hpp>

TEST(LoggerTests, LogInfoTest) {
    static_assert(std::is_same_v<rdmn::core::log::trivial_logger &&, decltype(RDMN_LOG(rdmn::core::log::LoggerType::info))>, "Logger type mismatch");
    RDMN_LOG(rdmn::core::log::LoggerType::info) << "Testing simple logger " << 1;
}