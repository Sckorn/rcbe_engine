#ifndef RCBE_TRANSFORM_HPP
#define RCBE_TRANSFORM_HPP

#include <data_types/math/Vector.hpp>
#include <data_types/math/Matrix.hpp>

namespace rcbe::math 
{
// TODO: refactor it so transform stores 4x4 matrix
class Transform
{
    using rotation_type =  Matrix3x3;
    using translation_type = Vector3d;
public:
    Transform() = default;
    Transform(const Matrix3x3 &r, const Vector3d &t);

    const rotation_type &getRotation() const;
    rotation_type &getRotation();

    const translation_type &getTranslation() const;
    translation_type &getTranslation();

    Vector3d transform(const Vector3d &v) const;
    Vector3d operator()(const Vector3d &v) const;
    friend Vector3d operator*(const Transform &t, const Vector3d &v);
    friend Transform operator*(const Transform &lhs, const Transform &rhs);
    friend Matrix3x3 operator*(const Transform &lhs, const Matrix3x3 &rhs);

private:
    rotation_type _rotation;
    translation_type _translation;
};
}

#endif