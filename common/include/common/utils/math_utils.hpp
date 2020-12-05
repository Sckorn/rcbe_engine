#ifndef RCBE_MATH_UTILS_HPP
#define RCBE_MATH_UTILS_HPP

#include <cmath>
#include <type_traits>
#include <concepts>

#include <rcbe-engine/fundamentals/types.hpp>

namespace rcbe::math
{
inline core::EngineScalar deg_to_rad(const core::EngineScalar angle) {
    return M_PI / 180 * angle;
}

inline core::EngineScalar rad_to_deg(const core::EngineScalar angle) {
    return 180 / M_PI * angle;
}

template <typename T>
concept IntegralValue = std::is_integral_v<T>;

template <IntegralValue Value>
inline bool even(const Value v) {
    return v % 2 == 0;
}

template <IntegralValue Value>
inline  bool odd(const Value v) {
    return !even(v);
}
}

#endif