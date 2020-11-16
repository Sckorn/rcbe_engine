#include <data_types/math/Transform.hpp>

namespace rcbe::math 
{
Transform::Transform(const Matrix3x3 &r, const Vector3d &t)
:
matrix_ {
    r.at(0, 0), r.at(0, 1), r.at(0, 2), t.x(),
    r.at(1, 0), r.at(1, 1), r.at(1, 2), t.y(),
    r.at(2, 0), r.at(2, 1), r.at(2, 2), t.z(),
    0, 0, 0, 1
}
{

}

Transform::Transform(const underlying_type& matrix)
:
matrix_ {
    matrix.at(0, 0), matrix.at(0, 1), matrix.at(0, 2), matrix.at(0, 3),
    matrix.at(1, 0), matrix.at(1, 1), matrix.at(1, 2), matrix.at(1, 3),
    matrix.at(2, 0), matrix.at(2, 1), matrix.at(2, 2), matrix.at(2, 3),
    matrix.at(3, 0), matrix.at(3, 1), matrix.at(3, 2), matrix.at(3, 3)
}
{

}

Vector3d Transform::transform(const Vector3d &v) const
{
    Vector3d ret;
    ret = getRotation() * v;
    ret = ret + getTranslation();
    return ret;
}

Vector3d Transform::operator()(const Vector3d &v) const
{
    return transform(v);
}

Vector3d operator*(const Transform &t, const Vector3d &v)
{
    return t.transform(v);
}

Transform operator*(const Transform &lhs, const Transform &rhs)
{
    const auto t = ::operator*(lhs.matrix_, rhs.matrix_);
    // TODO: fix namspaces to avoid this ^
    return Transform(t);
}

Matrix3x3 operator*(const Transform &lhs, const Matrix3x3 &rhs)
{
    // TODO: same as above
    return ::operator*(lhs.getRotation(), rhs);
}

Transform::rotation_type Transform::getRotation() const {
    const auto x_axis = matrix_.getColumn(0);
    const auto y_axis = matrix_.getColumn(1);
    const auto z_axis = matrix_.getColumn(2);

    return rotation_type {
            x_axis.x(), y_axis.x(), z_axis.x(),
            x_axis.y(), y_axis.y(), z_axis.y(),
            x_axis.z(), y_axis.z(), z_axis.z()
    };
}

Transform::translation_type Transform::getTranslation() const {
    const auto t = matrix_.getColumn(3);
    return translation_type{t.x(), t.y(), t.z()};
}

void Transform::inverse() {
    matrix_ = matrix_.inversed();
}

Transform Transform::inversed() const {
    auto copy = *this;
    copy.inverse();
    return copy;
}

const Transform::underlying_type& Transform::matrix() const {
    return matrix_;
}
}