#include <rdmn-engine/logger/trivial_logger.hpp>

#ifdef R_USE_BOOST_LOG
#include <boost/log/trivial.hpp>
#elif defined(R_USE_GLOG)
#include <glog/logging.h>
#endif

namespace rdmn::core::log {

R_PUBLIC_API void log_trace(std::string &&str) {
#ifdef R_USE_BOOST_LOG
    BOOST_LOG_TRIVIAL(trace) << str;
#elif defined(R_USE_GLOG)
    LOG(INFO) << str; // no trace in GLOG 
#endif
}

R_PUBLIC_API void log_info(std::string &&str) {
#ifdef R_USE_BOOST_LOG
    BOOST_LOG_TRIVIAL(info) << str;
#elif defined(R_USE_GLOG)
    LOG(INFO) << str;
#endif
}

R_PUBLIC_API void log_debug(std::string &&str) {
#ifdef R_USE_BOOST_LOG
    BOOST_LOG_TRIVIAL(debug) << str;
#elif defined(R_USE_GLOG)
    LOG(INFO) << str; // no trace in GLOG 
#endif
}

R_PUBLIC_API void log_warn(std::string &&str) {
#ifdef R_USE_BOOST_LOG
    BOOST_LOG_TRIVIAL(warn) << str;
#elif defined(R_USE_GLOG)
    LOG(WARNING) << str; // no trace in GLOG 
#endif
}

R_PUBLIC_API void log_error(std::string &&str) {
#ifdef R_USE_BOOST_LOG
    BOOST_LOG_TRIVIAL(error) << str;
#elif defined(R_USE_GLOG)
    LOG(ERROR) << str; // no trace in GLOG 
#endif
}

R_PUBLIC_API void log_fatal(std::string &&str) {
#ifdef R_USE_BOOST_LOG
    BOOST_LOG_TRIVIAL(error) << str;
#elif defined(R_USE_GLOG)
    LOG(ERROR) << str; // no trace in GLOG 
#endif
}

R_PUBLIC_API trivial_logger::trivial_logger(LoggerType type)
: type_{type} {}

R_PUBLIC_API trivial_logger::~trivial_logger() {
    switch(type_) {
        case LoggerType::info:
        log_info(iss_.str());
        break;
        default:
        break;
    };
}

R_PUBLIC_API trivial_logger &&get_trivial_logger(LoggerType type) {
    return std::move(trivial_logger(type));
}
}

R_PUBLIC_API rdmn::core::log::trivial_logger &&RDMN_LOG(rdmn::core::log::LoggerType type) {
    return get_trivial_logger(type);
}

R_PUBLIC_API rdmn::core::log::trivial_logger &rdmn::core::log::trivial_logger::operator<<(const char *v) {
    iss_ << std::string(v);
    return *this;
}