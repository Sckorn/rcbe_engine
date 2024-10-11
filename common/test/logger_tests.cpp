#include <gtest/gtest.h>
#include <glog/logging.h>

#include <rdmn-engine/logger/trivial_logger.hpp>

#include <rcbe-engine/utils/output_utils.hpp>

TEST(LoggerTests, LogInfoTest) {
    rcbe::utils::setup_logging(rdmn::core::log::LoggerType::info);
    RDMN_LOG(rdmn::core::log::LoggerType::info) << "Log info " << 1;
    RDMN_LOG(rdmn::core::log::LoggerType::error) << "Log error " << 1.1f << " " << 1.1;
    RDMN_LOG(rdmn::core::log::LoggerType::warn) << "Log Warn " << true << " " << 'c';
}