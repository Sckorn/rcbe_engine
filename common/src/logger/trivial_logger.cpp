#include <rdmn-engine/logger/trivial_logger.hpp>

#include <iostream>

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
    try { 
        switch(type_) {
            case LoggerType::info:
                log_info(iss_.str());
            break;
            case LoggerType::warn:
                log_warn(iss_.str());
            break;
            case LoggerType::error:
                log_error(iss_.str());
            break;
            case LoggerType::trace:
                log_trace(iss_.str());
            break;
            case LoggerType::debug:
                log_debug(iss_.str());
            break;
            case LoggerType::fatal:
                log_fatal(iss_.str());
            break;
            default:
            break;
        };
    } catch (const std::exception &e) {
        std::cerr << "Logger exception " << e.what() << std::endl;
    }
}

}



/*R_PUBLIC_API rdmn::core::log::trivial_logger &rdmn::core::log::trivial_logger::operator<<(const char *v) {
    printf("Start const char operator %s\n", v);
    std::string s = v;
    std::ostringstream oss{};
    printf("Start const char operator %s\n", s);
    iss_ << s;
    oss << "test";
    //iss_ << "dafuq";
    printf("Done const char operator %s\n", iss_.str());
    printf("Start const char operator %s\n", v);
    printf("OSS %s\n", std::string(oss.str()));
    return *this;
}*/