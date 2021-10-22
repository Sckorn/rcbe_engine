#ifndef RCBE_ENGINE_MATRIX_HELPERS_HPP
#define RCBE_ENGINE_MATRIX_HELPERS_HPP

#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/rotation_units.hpp>

namespace rcbe::math {
Matrix4x4 makePerspectiveMatrix(
        const core::EngineScalar near,
        const core::EngineScalar far,
        const math::deg fov,
        const core::Dimensions viewport);
}

#endif //RCBE_ENGINE_MATRIX_HELPERS_HPP
