#ifndef RDMN_LOGGER_TYPE_HPP
#define RDMN_LOGGER_TYPE_HPP

#include <cstdint>

namespace rdmn::core::log {
enum class LoggerType : uint8_t {
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    error = 4,
    fatal = 5
};
}

#endif // RDMN_LOGGER_TYPE_HPP