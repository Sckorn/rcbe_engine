#ifndef RCBE_MATH_UTILS_HPP
#define RCBE_MATH_UTILS_HPP

#include <cmath>
#include <type_traits>

#include <common/fundamentals/types.hpp>

namespace rcbe::math
{
inline core::EngineScalar deg_to_rad(const core::EngineScalar angle) {
    return M_PI / 180 * angle;
}

inline core::EngineScalar rad_to_deg(const core::EngineScalar angle) {
    return 180 / M_PI * angle;
}

template <typename Value, typename = std::enable_if_t<std::is_integral_v<Value>, void >>
inline bool even(const Value v) {
    return v % 2 == 0;
}

template <typename Value, typename = std::enable_if_t<std::is_integral_v<Value>, void >>
inline  bool odd(const Value v) {
    return !even(v);
}
}

#endif