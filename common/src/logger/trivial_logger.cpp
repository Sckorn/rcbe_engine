/**
 * Terminal colors explanation
 *
 * _____________________________________________________________
 *  | Parameter  | Effect                                      |
 *  ____________________________________________________________
 *  | 0          | reset all SGR effects to their default      |
 *  | 1          | bold or increased intensity                 |
 *  | 2          | faint or decreased intensity                |
 *  | 4          | singly underlined                           |
 *  | 5          | slow blink                                  |
 *  | 30-37      | foreground color (8 colors)                 |
 *  | 38;5;x     | foreground color (256 colors, non-standard) |
 *  | 38;2;r;g;b | foreground color (RGB, non-standard)        |
 *  | 40-47      | background color (8 colors)                 |
 *  | 48;5;x     | background color (256 colors, non-standard) |
 *  | 48;2;r;g;b | background color (RGB, non-standard)        |
 *  | 90-97      | bright foreground color (non-standard)      |
 *  | 100-107    | bright background color (non-standard)      |
 *  ____________________________________________________________
 *
 *  The 8 actual colors within the ranges (30-37, 40-47, 90-97, 100-107) are defined by the ANSI standard as follows:
 *  -----------------------------
 *  | Last Digit    |   Color   |
 *  -----------------------------
 *  | 0             |   black   |
 *  | 1             |   red     |
 *  | 2             |   green   |
 *  | 3             |   yellow  |
 *  | 4             |   blue    |
 *  | 5             |   magenta |
 *  | 6             |   cyan    |
 *  | 7             |   white   |
 *  -----------------------------
 *
 * */

#include <rdmn-engine/logger/trivial_logger.hpp>

#include <chrono>
#include <iostream>
#include <print>

namespace {
#ifdef _WIN32
std::string get_prefix_wnd(rdmn::core::log::LoggerType type) {
    using namespace rdmn::core::log;
    switch(type) {
        case LoggerType::info: {
            return "\033[32m";
        }
        break;
        case LoggerType::warn: {
            return "\033[93m";
        }
        break;
        case LoggerType::error: {
            return "\033[31m";
        }
        break;
        case LoggerType::trace: {
            return "\033[34m";
        }
        break;
        case LoggerType::debug: {
            return "\033[1;36m";
        }
        break;
        case LoggerType::fatal: {
            return "\033[41m";
        }
        break;
        default:
            return "";
        break;
    };
}
#endif
#ifdef __linux__
std::string get_prefix_lnx(rdmn::core::log::LoggerType type) {
    using namespace rdmn::core::log;
    switch(type) {
        case LoggerType::info: {
            return "\033[32m";
        }
        break;
        case LoggerType::warn: {
            return "\033[93m";
        }
        break;
        case LoggerType::error: {
            return "\033[31m";
        }
        break;
        case LoggerType::trace: {
            return "\033[34m";
        }
        break;
        case LoggerType::debug: {
            return "\033[36m";
        }
        break;
        case LoggerType::fatal: {
            return "\033[41m";
        }
        break;
        default:
            return "";
        break;
    };
}
#endif
std::string get_prefix(rdmn::core::log::LoggerType type) {
#ifdef __linux__
    return get_prefix_lnx(type);
#endif
#ifdef _WIN32
    return get_prefix_wnd(type);
#endif
}

std::string get_postfix() {
    return "\033[0m";
}

std::string get_time() {
    const auto &tzdb = std::chrono::get_tzdb();
    const auto curr_tz = tzdb.current_zone();
    const std::chrono::zoned_time zt{curr_tz->name(), std::chrono::system_clock::now()};
    std::ostringstream oss;
    oss << "[" << zt.get_local_time() << "] ";
    return oss.str();
}

std::string get_type_string(rdmn::core::log::LoggerType type) {
    using namespace rdmn::core::log;
    switch(type) {
        case LoggerType::info: {
            return " [info] ";
        }
        break;
        case LoggerType::warn: {
            return " [warning] ";
        }
        break;
        case LoggerType::error: {
            return " [error] ";
        }
        break;
        case LoggerType::trace: {
            return " [trace] ";
        }
        break;
        case LoggerType::debug: {
            return " [debug] ";
        }
        break;
        case LoggerType::fatal: {
            return " [fatal] ";
        }
        break;
        default:
            return "";
        break;
    };
}

void log_impl(std::string &&str, rdmn::core::log::LoggerType type) {
    std::println(
        "{}{}{}{}{}", 
        get_time(),
        get_prefix(type), 
        get_type_string(type),
        str, 
        get_postfix()
    );
}
}

namespace rdmn::core::log {

R_PUBLIC_API void log_trace(std::string &&str) noexcept {
    log_impl(std::move(str), rdmn::core::log::LoggerType::trace);
}

R_PUBLIC_API void log_info(std::string &&str) noexcept {
    log_impl(std::move(str), rdmn::core::log::LoggerType::info);
}

R_PUBLIC_API void log_debug(std::string &&str) noexcept {
    log_impl(std::move(str), rdmn::core::log::LoggerType::debug);
}

R_PUBLIC_API void log_warn(std::string &&str) noexcept {
    log_impl(std::move(str), rdmn::core::log::LoggerType::warn);
}

R_PUBLIC_API void log_error(std::string &&str) noexcept {
    log_impl(std::move(str), rdmn::core::log::LoggerType::error);
}

R_PUBLIC_API void log_fatal(std::string &&str) noexcept {
    log_impl(std::move(str), rdmn::core::log::LoggerType::fatal);
}

R_PUBLIC_API trivial_logger::trivial_logger(LoggerType type)
: type_{type} {}

R_PUBLIC_API trivial_logger::~trivial_logger() {
    if (type_ >= log_level_) {
        try { 
            switch(type_) {
                case LoggerType::info: {
                    log_info(iss_.str());
                }
                break;
                case LoggerType::warn: {
                    log_warn(iss_.str());
                }
                break;
                case LoggerType::error: {
                    log_error(iss_.str());
                }
                break;
                case LoggerType::trace: {
                    log_trace(iss_.str());
                }
                break;
                case LoggerType::debug: {
                    log_debug(iss_.str());
                }
                break;
                case LoggerType::fatal: {
                    log_fatal(iss_.str());
                }
                break;
                default:
                break;
            };
        } catch (const std::exception &e) {
            std::cerr << "Logger exception " << e.what() << std::endl;
        }
    }
}

R_PUBLIC_API void trivial_logger::set_log_level(LoggerType level) {
    trivial_logger::log_level_ = level;
}

LoggerType trivial_logger::log_level_ = LoggerType::info;

}