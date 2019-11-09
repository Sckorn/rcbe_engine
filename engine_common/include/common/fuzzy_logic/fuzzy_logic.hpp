#ifndef RCBE_FUZZY_LOGIC_HPP
#define RCBE_FUZZY_LOGIC_HPP

#include <type_traits>
#include <cmath>

#include <common/fundamentals/constants.hpp>

namespace rcbe::core
{
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
bool fuzzy_equal(const T lhs, const T rhs, const double epsilon = core::EPSILON)
{
    return (std::abs(lhs - rhs) < epsilon);
}
}

#endif