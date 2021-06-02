#include <cmath>

#include <rcbe-engine/datamodel/math/matrix_helpers.hpp>

namespace rcbe::math {
Matrix4x4 makePerspectiveMatrix(
        const core::EngineScalar near,
        const core::EngineScalar far,
        const math::deg fov,
        const core::Dimensions viewport) {

    const auto aspect = static_cast<float>(viewport.width) / static_cast<float>(viewport.height);
    const auto fovy_rad = fov.as_rad();

    const auto tan = std::tan(static_cast<const double>(fovy_rad) / 2);

    if (tan == 0)
        throw std::runtime_error("Can't compute cotangent");

    Matrix4x4 ret {
        1 / (tan * aspect), 0, 0, 0,
        0, 1 / tan, 0, 0,
        0, 0, -((far + near)/(far - near)), -(2 * far * near) / (far - near),
        0, 0, -1, 0
    };

    return ret;
}
}