#include <data_types/math/Transform.hpp>

namespace rcbe::math 
{
Transform::Transform(const Matrix3x3 &r, const Vector3d &t)
:
_rotation { r }
,_translation { t }
{

}

Vector3d Transform::transform(const Vector3d &v) const
{
    Vector3d ret;
    ret = _rotation * v;
    ret = ret + _translation;
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
    Transform ret;

    ret._rotation = ::operator*(lhs._rotation, rhs._rotation);
    // TODO: fix namspaces to avoid this ^
    ret._translation = lhs._translation + rhs._translation;

    return ret;
}

Matrix3x3 operator*(const Transform &lhs, const Matrix3x3 &rhs)
{
    return ::operator*(lhs.getRotation(), rhs);
}

const Transform::rotation_type &Transform::getRotation() const
{
    return _rotation;
}

Transform::rotation_type &Transform::getRotation()
{
    return _rotation;
}

const Transform::translation_type &Transform::getTranslation() const
{
    return _translation;
}

Transform::translation_type &Transform::getTranslation()
{
    return _translation;
}
}