#ifndef RDMN_ENGINE_MATRIX_HELPERS_HPP
#define RDMN_ENGINE_MATRIX_HELPERS_HPP

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/rotation_units.hpp>
#include <rcbe-engine/fundamentals/types.hpp>

namespace rdmn::render {
R_PUBLIC_API rcbe::math::Matrix4x4 makePerspectiveMatrix(
    const rcbe::core::EngineScalar near,
    const rcbe::core::EngineScalar far,
    const rcbe::math::deg fov,
    const rcbe::core::IntegralDimensions viewport);
}

#endif//RDMN_ENGINE_MATRIX_HELPERS_HPP
