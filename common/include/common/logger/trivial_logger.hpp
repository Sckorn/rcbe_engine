#ifndef RDMN_COMMON_TRIVIAL_LOGGER_HPP
#define RDMN_COMMON_TRIVIAL_LOGGER_HPP

#include <sstream>
#include <iostream>

#include <rdmn-engine/public_api.hpp>
#include <rdmn-engine/logger/LoggerType.hpp>

namespace rdmn::core::log {

R_PUBLIC_API void log_trace(std::string &&str);
R_PUBLIC_API void log_info(std::string &&str);
R_PUBLIC_API void log_debug(std::string &&str);
R_PUBLIC_API void log_warn(std::string &&str);
R_PUBLIC_API void log_error(std::string &&str);
R_PUBLIC_API void log_fatal(std::string &&str);

struct trivial_logger {
    R_PUBLIC_API trivial_logger(LoggerType type);
    R_PUBLIC_API ~trivial_logger();

    R_PUBLIC_API trivial_logger(const trivial_logger &other) = delete;
    R_PUBLIC_API trivial_logger &operator=(const trivial_logger &other) = delete;

    R_PUBLIC_API trivial_logger(trivial_logger &&other) = delete;
    R_PUBLIC_API trivial_logger &operator=(trivial_logger &&other) = delete;

    template <typename Val>
    R_PUBLIC_API trivial_logger &&operator<<(Val &&v) && {
        iss_ << v;
        return std::move(*this);
    }

    template <size_t Size>
    R_PUBLIC_API trivial_logger &&operator<<(const char (&cstr)[Size]) && {
        for (size_t i = 0; i < Size - 1; ++i) {
            iss_ << cstr[i];
        }

        if (cstr[Size - 1] != '\0')
            iss_ << '\0';

        return std::move(*this);
    }

    R_PUBLIC_API trivial_logger &&operator<<(bool &&v) && {
        iss_ << std::boolalpha << v;
        return std::move(*this);
    }

private:
    std::ostringstream iss_ {};
    LoggerType type_;
};

}

using RDMN_LOG = rdmn::core::log::trivial_logger;


#endif // RDMN_COMMON_TRIVIAL_LOGGER_HPP
