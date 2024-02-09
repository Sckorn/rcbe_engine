#ifndef RCBE_ENGINE_MATRIX_HELPERS_HPP
#define RCBE_ENGINE_MATRIX_HELPERS_HPP

#include <cmath>
#include <tuple>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/euler_angles.hpp>
#include <rcbe-engine/datamodel/math/rotation_units.hpp>
#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/fuzzy_logic/fuzzy_logic.hpp>

namespace rcbe::math {

/// Roll is Z, pitch is X, yaw is Y
Matrix4x4 matrixFromZXY(const roll r, const pitch p, const yaw y);

/// Our euler rotations are ZXY
template <typename V>
std::tuple<yaw, pitch, roll> yprFromMatrix(const math::Matrix<V, 4, 4> &m) {
    const auto rm = m.transposed();
    math::roll r {math::rad {std::atan(-rm.at(0, 1) / rm.at(1, 1))}};
    math::pitch p {math::rad {std::atan(rm.at(2, 1) / std::sqrt(1 - rm.at(2, 1) * rm.at(2, 1)))}};
    math::yaw y {math::rad {std::atan(-rm.at(2, 0) / rm.at(2, 2))}};

    return {y, p, r};
}

template <typename V>
std::tuple<yaw, pitch, roll> yprFromMatrix(const Matrix<V, 3, 3> &m) {
    return yprFromMatrix<V>(Matrix<V, 4, 4> {
        m.at(0, 0), m.at(0, 1), m.at(0, 2), 0,
        m.at(1, 0), m.at(1, 1), m.at(1, 2), 0,
        m.at(2, 0), m.at(2, 1), m.at(2, 2), 0,
        0, 0, 0, 1});
}
}// namespace rcbe::math

#endif//RCBE_ENGINE_MATRIX_HELPERS_HPP
