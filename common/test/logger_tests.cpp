#include <gtest/gtest.h>
#include <glog/logging.h>

#include <rdmn-engine/logger/trivial_logger.hpp>

TEST(LoggerTests, LogInfoTest) {
    RDMN_LOG(rdmn::core::log::LoggerType::info) << "Log info " << 1;
    RDMN_LOG(rdmn::core::log::LoggerType::error) << "Log error " << 1.0f << " " << 1.0;
    RDMN_LOG(rdmn::core::log::LoggerType::warn) << "Log Warn " << true << " " << 'c';
}