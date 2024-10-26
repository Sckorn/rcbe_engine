#ifndef RDMN_LOGGER_TYPE_HPP
#define RDMN_LOGGER_TYPE_HPP

#include <cstdint>

namespace rdmn::core::log {
enum class LoggerType : uint8_t {
    info,
    debug,
    trace,
    error,
    warn,
    fatal
};
}

#endif // RDMN_LOGGER_TYPE_HPP