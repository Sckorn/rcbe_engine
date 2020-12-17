#ifndef RCBE_MATH_HPP
#define RCBE_MATH_HPP

#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::math
{
static constexpr Vector3d WORLD_UP(0., 1., 0.);
static constexpr Vector3d WORLD_FORWARD(0., 0., -1.);
static constexpr Vector3d WORLD_RIGHT(1.0, 0., 0.);
}

#endif