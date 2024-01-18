#ifndef RCBE_ENGINE_OUTPUT_UTILS_HPP
#define RCBE_ENGINE_OUTPUT_UTILS_HPP

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

namespace rcbe::utils {
void setup_logging(boost::log::trivial::severity_level minimal_visible_level = boost::log::trivial::info);
}

#endif//RCBE_ENGINE_OUTPUT_UTILS_HPP
