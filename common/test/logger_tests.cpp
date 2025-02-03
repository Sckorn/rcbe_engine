#include <gtest/gtest.h>

#include <memory>

#include <rdmn-engine/logger/trivial_logger.hpp>

TEST(LoggerTests, LogInfoTest) {
    rdmn::core::log::trivial_logger::set_log_level(rdmn::core::log::LoggerType::trace);
    RDMN_LOG(RDMN_LOG_INFO) << "Log info " << 1;
    RDMN_LOG(RDMN_LOG_ERROR) << "Log error " << 1.1f << " " << 1.1;
    RDMN_LOG(RDMN_LOG_WARN) << "Log Warn " << true << " " << 'c';
    RDMN_LOG(RDMN_LOG_INFO) << "Loginfo" << 1;

    size_t i = 0;
    RDMN_LOG(RDMN_LOG_TRACE) << "Log trace " << std::string("string");
    RDMN_LOG(RDMN_LOG_DEBUG) << "Log debug " << std::addressof(i);
    RDMN_LOG(RDMN_LOG_FATAL) << "Log fatal " << true << " " << 'c';

    RDMN_LOG(RDMN_LOG_INFO) << "Log just literal string";
}