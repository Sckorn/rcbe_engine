#ifndef RCBE_MATH_HPP
#define RCBE_MATH_HPP

#include <data_types/config/data_model_config.hpp>
#include <data_types/math/Vector.hpp>
#include <data_types/math/Matrix.hpp>

namespace rcbe::math
{
using Matrix3x3d = Matrix<core::EngineScalar, 3, 3>;
}

#endif