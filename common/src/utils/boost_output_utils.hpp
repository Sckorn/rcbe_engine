#ifndef RDMN_BOOST_OUTPUT_UTILS_HPP
#define RDMN_BOOST_OUTPUT_UTILS_HPP

#include <boost/core/null_deleter.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/formatters/named_scope.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID", boost::log::attributes::current_thread_id::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(process_id, "ProcessID", boost::log::attributes::current_process_id::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", boost::log::attributes::named_scope::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)

namespace {
void coloring_formatter(boost::log::record_view const &rec, boost::log::formatting_ostream &strm) {
    namespace logging_expr = boost::log::expressions;

    auto severity = rec[boost::log::trivial::severity];
    if (severity) {
        // Set the color
        switch (severity.get()) {
            case boost::log::trivial::info:
                strm << "\033[32m";
                break;
            case boost::log::trivial::warning:
                strm << "\033[93m";
                break;
            case boost::log::trivial::error:
                strm << "\033[31m";
                break;
            case boost::log::trivial::fatal:
                strm << "\033[41m";
                break;
            case boost::log::trivial::debug:
                strm << "\033[36m";
                break;
            case boost::log::trivial::trace:
                strm << "\033[34m";
                break;
            default:
                break;
        }
    }

    strm << " [" << rec[boost::log::trivial::severity] << "] " << rec[boost::log::expressions::smessage];
    if (severity) {
        // Restore the default color
        strm << "\033[0m";
    }
}

boost::log::trivial::severity_level convert_severity(rdmn::core::log::LoggerType severity) {
    switch (severity) {
        case rdmn::core::log::LoggerType::info:
            return boost::log::trivial::info;
        case rdmn::core::log::LoggerType::warn:
            return boost::log::trivial::warning;
        case rdmn::core::log::LoggerType::error:
            return boost::log::trivial::error;
        case rdmn::core::log::LoggerType::trace:
            return boost::log::trivial::trace;
        case rdmn::core::log::LoggerType::debug:
            return boost::log::trivial::debug;
        case rdmn::core::log::LoggerType::fatal:
            return boost::log::trivial::fatal;
    }
}

}// namespace

namespace rcbe::utils {

void setup_logging(boost::log::trivial::severity_level minimal_visible_level) {
    BOOST_LOG_FUNCTION();

    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= minimal_visible_level);

    namespace logging_expr = boost::log::expressions;
    namespace logging_sinks = boost::log::sinks;
    boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
    boost::log::core::get()->add_global_attribute("LineID", boost::log::attributes::counter<unsigned int>(1));
    boost::log::core::get()->add_global_attribute("ThreadID", boost::log::attributes::current_thread_id());
    boost::log::core::get()->add_global_attribute("ProcessID", boost::log::attributes::current_process_id());
    boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
    boost::log::core::get()->add_thread_attribute("Scope", boost::log::attributes::named_scope());

    boost::shared_ptr<logging_sinks::synchronous_sink<logging_sinks::text_ostream_backend>> stdout_sink {
        boost::make_shared<logging_sinks::synchronous_sink<logging_sinks::text_ostream_backend>>()};
    stdout_sink->locked_backend()->auto_flush(true);
    stdout_sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::cout, boost::null_deleter {}));

    boost::log::formatter stdout_formatter {
        logging_expr::stream
        << logging_expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "[%Y-%m-%d %H:%M:%S.%f] ")
        << logging_expr::wrap_formatter(&coloring_formatter)};

    stdout_sink->set_formatter(stdout_formatter);
    stdout_sink->set_filter(boost::log::trivial::severity >= minimal_visible_level);

    // "register" sink
    boost::log::core::get()->add_sink(stdout_sink);
}

R_PUBLIC_API void setup_logging(rdmn::core::log::LoggerType severity) {
    setup_logging(convert_severity(severity));
}

}// namespace rcbe::utils

#endif