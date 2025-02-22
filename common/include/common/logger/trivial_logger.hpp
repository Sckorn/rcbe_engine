#ifndef RDMN_COMMON_TRIVIAL_LOGGER_HPP
#define RDMN_COMMON_TRIVIAL_LOGGER_HPP

#include <sstream>

#include <rdmn-engine/logger/LoggerType.hpp>

namespace rdmn::core::log {

/**
 * @brief 
 * Current trivial_logger implementation is created to mimic Boost::Log semantics
 * in order to reduce the needed changes when doing refactoring of many,
 * and I mean MANY lines of logging code.
 * It is 
 * - not efficient
 * - not robust
 * - not practical
 * - and overall a necessary evil
 * 
 * This implementation will be replaced by a proper logger, which will be supplied to classes which require it,
 * when propper logging refactoring is done.
 * 
 */

R_PUBLIC_API void log_trace(std::string &&str) noexcept;
R_PUBLIC_API void log_info(std::string &&str) noexcept;
R_PUBLIC_API void log_debug(std::string &&str) noexcept;
R_PUBLIC_API void log_warn(std::string &&str) noexcept;
R_PUBLIC_API void log_error(std::string &&str) noexcept;
R_PUBLIC_API void log_fatal(std::string &&str) noexcept;

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
        for (size_t i = 0; i < Size - 1; ++i) { // Size - 1 removes trailing space
            iss_ << cstr[i];
        }

        return std::move(*this);
    }

    R_PUBLIC_API trivial_logger &&operator<<(bool &&v) && {
        iss_ << std::boolalpha << v;
        return std::move(*this);
    }

    R_PUBLIC_API static void set_log_level(LoggerType level);

private:
    static LoggerType log_level_;
    std::ostringstream iss_ {};
    LoggerType type_;
};

}

using RDMN_LOG = rdmn::core::log::trivial_logger;

inline constexpr auto RDMN_LOG_INFO = rdmn::core::log::LoggerType::info;
inline constexpr auto RDMN_LOG_WARN = rdmn::core::log::LoggerType::warn;
inline constexpr auto RDMN_LOG_ERROR = rdmn::core::log::LoggerType::error;
inline constexpr auto RDMN_LOG_DEBUG = rdmn::core::log::LoggerType::debug;
inline constexpr auto RDMN_LOG_TRACE = rdmn::core::log::LoggerType::trace;
inline constexpr auto RDMN_LOG_FATAL = rdmn::core::log::LoggerType::fatal;


#endif // RDMN_COMMON_TRIVIAL_LOGGER_HPP
