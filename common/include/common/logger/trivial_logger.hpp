#ifndef RDMN_COMMON_TRIVIAL_LOGGER_HPP
#define RDMN_COMMON_TRIVIAL_LOGGER_HPP

#include <iostream>
#include <cstdint>
#include <sstream>

#include <rdmn-engine/public_api.hpp>

namespace rdmn::core::log {

enum class LoggerType : uint8_t {
    info,
    debug,
    trace,
    error,
    warn,
    fatal
};

//namespace {

R_PUBLIC_API void log_trace(std::string &&str);
R_PUBLIC_API void log_info(std::string &&str);
R_PUBLIC_API void log_debug(std::string &&str);
R_PUBLIC_API void log_warn(std::string &&str);
R_PUBLIC_API void log_error(std::string &&str);
R_PUBLIC_API void log_fatal(std::string &&str);
//}

struct trivial_logger {
    R_PUBLIC_API trivial_logger(LoggerType type);
    R_PUBLIC_API ~trivial_logger();

    template <typename Val>
    R_PUBLIC_API trivial_logger &operator<<(Val &&v) {
        iss_ << std::to_string(v);
        return *this;
    }

    template <size_t Size>
    R_PUBLIC_API trivial_logger &operator<<(const char (&cstr)[Size]) {
        for (size_t i = 0; i < Size; ++i) {
            iss_ << cstr[i];
        }
        if (cstr[Size] != '\0')
            iss_ << '\0';
        return *this;
    }

    R_PUBLIC_API trivial_logger &operator<<(char &&v) {
        iss_ << v;
        return *this;
    }

private:
    std::ostringstream iss_ {};
    LoggerType type_;
};
//}

}

using RDMN_LOG = rdmn::core::log::trivial_logger;


#endif // RDMN_COMMON_TRIVIAL_LOGGER_HPP
