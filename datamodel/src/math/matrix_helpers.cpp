#include <cmath>

#include <rcbe-engine/datamodel/math/matrix_helpers.hpp>

namespace rcbe::math {

R_PUBLIC_API Matrix4x4 matrixFromZXY(const roll r, const pitch p, const yaw y) {
    const auto cr = std::cos(static_cast<double>(r.as_rad()));
    const auto sr = std::sin(static_cast<double>(r.as_rad()));
    const auto cp = std::cos(static_cast<double>(p.as_rad()));
    const auto sp = std::sin(static_cast<double>(p.as_rad()));
    const auto cy = std::cos(static_cast<double>(y.as_rad()));
    const auto sy = std::sin(static_cast<double>(y.as_rad()));

    return Matrix4x4 {
        cr * cy - sr * sp * sy, cy * sr + cr * sp * sy, -cp * sy, 0,
        -cp * sr, cr * cp, sp, 0,
        cr * sy + cy * sr * sp, sr * sy - cr * cy * sp, cr * cy, 0,
        0, 0, 0, 1};
}
}// namespace rcbe::math