#ifndef RCBE_FUZZY_LOGIC_HPP
#define RCBE_FUZZY_LOGIC_HPP

#include <type_traits>
#include <cmath>
#include <concepts>

#include <rcbe-engine/fundamentals/constants.hpp>

namespace rcbe::core
{
template <typename T>
concept FloatingPointValue = std::is_floating_point_v<T>;

template <FloatingPointValue T>
bool fuzzy_equal(const T lhs, const T rhs, const double epsilon = core::EPSILON)
{
    return (std::abs(lhs - rhs) < epsilon);
}
}

#endif