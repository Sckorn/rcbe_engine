#ifndef RCBE_MATH_HPP
#define RCBE_MATH_HPP

#include <data_types/config/data_model_config.hpp>
#include <data_types/math/Vector.hpp>

namespace rcbe::math
{
static const Vector3d WORLD_UP(0., 1., 0.);
static const Vector3d WORLD_FORWARD(0., 0., -1.);
static const Vector3d WORLD_RIGHT(1.0, 0., 0.);
}

#endif