#ifndef RCBE_TRANSFORM_HPP
#define RCBE_TRANSFORM_HPP

#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rdmn-engine/public_api.hpp>

namespace rcbe::math {
class Transform {
public:

    using RotationType = Matrix3x3;
    using TranslationType = Vector3d;
    using UnderlyingType = Matrix4x4;

    R_PUBLIC_API Transform() = default;
    R_PUBLIC_API Transform(const RotationType &r, const TranslationType &t);
    R_PUBLIC_API explicit Transform(const UnderlyingType &matrix);

    [[nodiscard]] R_PUBLIC_API RotationType getRotation() const;
    [[nodiscard]] R_PUBLIC_API TranslationType getTranslation() const;

    [[nodiscard]] R_PUBLIC_API Vector3d transform(const Vector3d &v) const;
    R_PUBLIC_API Vector3d operator()(const Vector3d &v) const;
    friend R_PUBLIC_API Vector3d operator*(const Transform &t, const Vector3d &v);
    friend R_PUBLIC_API Transform operator*(const Transform &lhs, const Transform &rhs);
    friend R_PUBLIC_API Matrix3x3 operator*(const Transform &lhs, const Matrix3x3 &rhs);

    R_PUBLIC_API void inverse();
    [[nodiscard]] R_PUBLIC_API Transform inversed() const;

    [[nodiscard]] const R_PUBLIC_API UnderlyingType &matrix() const;

private:

    UnderlyingType matrix_;
};
}// namespace rcbe::math

#endif