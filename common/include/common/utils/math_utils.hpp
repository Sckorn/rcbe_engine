#ifndef RCBE_MATH_UTILS_HPP
#define RCBE_MATH_UTILS_HPP

#include <common/fundamentals/types.hpp>
#include <cmath>

namespace rcbe::core
{
inline EngineScalar deg_to_rad(const EngineScalar angle)
{
    return M_PI / 180 * angle ;
}
}

#endif