#ifndef RCBE_QUATERNION_HPP
#define RCBE_QUATERNION_HPP

#include <cmath>

#include <data_types/math/Vector.hpp>

#include <common/utils/math_utils.hpp>

namespace rcbe::math
{
template <typename ValueType>
class Quaternion
{
public:
    static constexpr size_t dimension = 4;
    using value_type = ValueType;
    using vector_type = Vector<value_type, dimension - 1>;
    using storage_type = Vector<value_type, dimension>;

    Quaternion(const Vector3d &v, const value_type s)
    :
    q_ { v.x(), v.y(), v.z(), s }
    {}

    Quaternion(const value_type yaw, const value_type pitch, const value_type roll)
    {
       auto r = rcbe::math::deg_to_rad(roll);
       auto p = rcbe::math::deg_to_rad(pitch);
       auto y = rcbe::math::deg_to_rad(yaw);

       auto qx = std::sin(r / 2) * std::cos(p / 2) * std::cos(y / 2) - std::cos(r / 2) * std::sin(p / 2) * std::sin(y / 2);
       auto qy = std::cos(r / 2) * std::sin(p / 2) * std::cos(y / 2) + std::sin(r / 2) * std::cos(p / 2) * std::sin(y / 2);
       auto qz = std::cos(r / 2) * std::cos(p / 2) * std::sin(y / 2) - std::sin(r / 2) * std::sin(p / 2) * std::cos(y / 2);
       auto qw = std::cos(r / 2) * std::cos(p / 2) * std::cos(y / 2) + std::sin(r / 2) * std::sin(p / 2) * std::sin(y / 2);

       q_ = storage_type {qx, qy, qz, qw};
    }

    value_type &x()
    {
        return q_.x();
    }

    const value_type &x() const
    {
        return q_.x();
    }

    value_type &y()
    {
        return q_.y();
    }

    const value_type &y() const
    {
        return q_.y();
    }

    value_type &z()
    {
        return q_.z();
    }

    const value_type &z() const
    {
        return q_.z();
    }

    value_type &w()
    {
        return q_.w();
    }

    const value_type &w() const
    {
        return q_.w();
    }

    vector_type getVectorPart() const
    {
        return {q_.x(), q_.y(), q_.z()};
    }

    value_type getScalarPart() const
    {
        return q_.w();
    }

    const value_type &at(const size_t index) const 
    {
        return q_.at(index);
    }

    value_type &operator[](const size_t index)
    {
        return q_[index];
    }

    void operator+=(const Quaternion &q)
    {
        q_ += q.q_;
    }

    value_type norm() const
    {
        auto scalar = w() * w();
        auto vec = getVectorPart();

        auto v = vec * vec;

        return std::sqrt(scalar + v);
    }

    void normalize()
    {
        auto norm_v = norm();
        if (norm_v != 0)
        {
            norm_v = 1 / norm_v;

            q_[3] *= norm_v;

            q_[0] *= norm_v;
            q_[1] *= norm_v;
            q_[2] *= norm_v;
        }
    }

    Quaternion normalized() const
    {
        auto copy = *this;

        copy.normalize();
        return copy;
    }

    friend Quaternion operator+(const Quaternion &q1, const Quaternion &q2)
    {
        return { q1.getVectorPart() + q2.getVectorPart(), q1.getScalarPart() + q2.getScalarPart() };
    }

    friend Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
    {
        auto vec1 = q1.getVectorPart();
        auto vec2 = q2.getVectorPart();

        auto scal1 = q1.getScalarPart();
        auto scal2 = q2.getScalarPart();

        auto scalar = scal1 * scal2 - vec1 * vec2;
        auto vec = scal1 * vec2 + scal2 * vec1 + Quaternion::vector_type::cross(vec1, vec2);

        return {vec, scalar};
    }

    Quaternion conjugate() const 
    {
        auto scalar = w();
        auto vec = getVectorPart() * -1;
        return {vec, scalar};
    }

    Quaternion inverse() const
    {
        auto norm_val = norm();
        norm_val *= norm_val;
        norm_val = 1 / norm_val;

        auto conj = conjugate();

        auto scalar = conj.w() * norm_val;
        auto tmp  = getVectorPart();
        auto vec = tmp * norm_val;

        return {vec, scalar};
    }

private:
    storage_type q_;
};

using Quaterniond = Quaternion<core::EngineScalar>;
}

#endif