#ifndef RCBE_GEOMETRY_HPP
#define RCBE_GEOMETRY_HPP

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <iostream>

#include <data_types/data_model_config.hpp>
#include <data_types/geometry/Vector.hpp>
#include <data_types/geometry/Matrix.hpp>

namespace rcbe::geometry
{
using Vector3d = Vector<core::EngineScalar, 3>;
using Vector2d = Vector<core::EngineScalar, 2>;

using Matrix3x3d = Matrix<double, 3, 3>;
}

#endif