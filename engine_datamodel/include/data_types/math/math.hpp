#ifndef RCBE_MATH_HPP
#define RCBE_MATH_HPP

#include <data_types/data_model_config.hpp>
#include <data_types/math/Vector.hpp>
#include <data_types/math/Matrix.hpp>

namespace rcbe::math
{
using Vector3d = Vector<core::EngineScalar, 3>;
using Vector2d = Vector<core::EngineScalar, 2>;

using Matrix3x3d = Matrix<core::EngineScalar, 3, 3>;
}

#endif