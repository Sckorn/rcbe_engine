#ifndef RDMN_COMMON_TRIVIAL_LOGGER_HPP
#define RDMN_COMMON_TRIVIAL_LOGGER_HPP

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

    /*template <typename Val>
    friend trivial_logger &operator<<(trivial_logger &tl, Val &&v) {
        tl.iss_ << std::to_string(v);
        return tl;
    }*/

    template <typename Val>
    R_PUBLIC_API trivial_logger &operator<<(Val &&v) {
        iss_ << std::to_string(v);
        return *this;
    }

    R_PUBLIC_API trivial_logger &operator<<(const char *v);

private:
    std::stringstream iss_{};
    LoggerType type_;

};
//}

R_PUBLIC_API [[nodiscard]] trivial_logger &&get_trivial_logger(LoggerType type);

}

R_PUBLIC_API [[nodiscard]] rdmn::core::log::trivial_logger &&RDMN_LOG(rdmn::core::log::LoggerType type);

#endif // RDMN_COMMON_TRIVIAL_LOGGER_HPP
