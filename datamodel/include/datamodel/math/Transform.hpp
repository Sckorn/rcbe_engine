#ifndef RCBE_TRANSFORM_HPP
#define RCBE_TRANSFORM_HPP

#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::math {
class R_PUBLIC_API Transform {
public:

    using RotationType = Matrix3x3;
    using TranslationType = Vector3d;
    using UnderlyingType = Matrix4x4;

    Transform() = default;
    Transform(const RotationType &r, const TranslationType &t);
    explicit Transform(const UnderlyingType &matrix);

    [[nodiscard]] RotationType getRotation() const;
    [[nodiscard]] TranslationType getTranslation() const;

    [[nodiscard]] Vector3d transform(const Vector3d &v) const;
    Vector3d operator()(const Vector3d &v) const;
    friend R_PUBLIC_API Vector3d operator*(const Transform &t, const Vector3d &v);
    friend R_PUBLIC_API Transform operator*(const Transform &lhs, const Transform &rhs);
    friend R_PUBLIC_API Matrix3x3 operator*(const Transform &lhs, const Matrix3x3 &rhs);

    void inverse();
    [[nodiscard]] Transform inversed() const;

    [[nodiscard]] const UnderlyingType &matrix() const;

private:

    UnderlyingType matrix_;
};
}// namespace rcbe::math

#endif