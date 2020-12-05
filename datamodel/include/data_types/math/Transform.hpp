#ifndef RCBE_TRANSFORM_HPP
#define RCBE_TRANSFORM_HPP

#include <data_types/math/Vector.hpp>
#include <data_types/math/Matrix.hpp>

namespace rcbe::math 
{
class Transform
{
public:
    using rotation_type =  Matrix3x3;
    using translation_type = Vector3d;
    using underlying_type = Matrix4x4;

    Transform() = default;
    Transform(const rotation_type &r, const translation_type &t);
    explicit Transform(const underlying_type& matrix);

    [[nodiscard]] rotation_type getRotation() const;
    [[nodiscard]] translation_type getTranslation() const;

    [[nodiscard]]Vector3d transform(const Vector3d &v) const;
    Vector3d operator()(const Vector3d &v) const;
    friend Vector3d operator*(const Transform &t, const Vector3d &v);
    friend Transform operator*(const Transform &lhs, const Transform &rhs);
    friend Matrix3x3 operator*(const Transform &lhs, const Matrix3x3 &rhs);

    void inverse();
    [[nodiscard]] Transform inversed() const;

    [[nodiscard]] const underlying_type& matrix() const;

private:

    underlying_type matrix_;
};
}

#endif