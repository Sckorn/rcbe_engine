#ifndef RCBE_QUATERNION_HPP
#define RCBE_QUATERNION_HPP

#include <cmath>

#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/math/euler_angles.hpp>
#include <rcbe-engine/utils/math_utils.hpp>

namespace rcbe::math {
template <typename Value>
class Quaternion {
public:

    static constexpr size_t DIMENSION = 4;
    using ValueType = Value;
    using VectorType = Vector<ValueType, DIMENSION - 1>;
    using StorageType = Vector<ValueType, DIMENSION>;

    Quaternion(const Vector3d &v, const ValueType s)
        : q_ {v.x(), v.y(), v.z(), s} {}

    Quaternion(const yaw &y_, const pitch &p_, const roll &r_) {
        auto r = static_cast<double>(r_.as_rad());
        auto p = static_cast<double>(p_.as_rad());
        auto y = static_cast<double>(y_.as_rad());

        auto qx = std::sin(r / 2) * std::cos(p / 2) * std::cos(y / 2) - std::cos(r / 2) * std::sin(p / 2) * std::sin(y / 2);
        auto qy = std::cos(r / 2) * std::sin(p / 2) * std::cos(y / 2) + std::sin(r / 2) * std::cos(p / 2) * std::sin(y / 2);
        auto qz = std::cos(r / 2) * std::cos(p / 2) * std::sin(y / 2) - std::sin(r / 2) * std::sin(p / 2) * std::cos(y / 2);
        auto qw = std::cos(r / 2) * std::cos(p / 2) * std::cos(y / 2) + std::sin(r / 2) * std::sin(p / 2) * std::sin(y / 2);

        q_ = StorageType {qx, qy, qz, qw};
    }

    ValueType &x() {
        return q_.x();
    }

    const ValueType &x() const {
        return q_.x();
    }

    ValueType &y() {
        return q_.y();
    }

    const ValueType &y() const {
        return q_.y();
    }

    ValueType &z() {
        return q_.z();
    }

    const ValueType &z() const {
        return q_.z();
    }

    ValueType &w() {
        return q_.w();
    }

    const ValueType &w() const {
        return q_.w();
    }

    [[nodiscard]] VectorType getVectorPart() const {
        return {q_.x(), q_.y(), q_.z()};
    }

    [[nodiscard]] ValueType getScalarPart() const {
        return q_.w();
    }

    const ValueType &at(const size_t index) const {
        return q_.at(index);
    }

    ValueType &operator[](const size_t index) {
        return q_[index];
    }

    void operator+=(const Quaternion &q) {
        q_ += q.q_;
    }

    ValueType norm() const {
        auto scalar = w() * w();
        auto vec = getVectorPart();

        auto v = vec * vec;

        return std::sqrt(scalar + v);
    }

    void normalize() {
        auto norm_v = norm();
        if (norm_v != 0) {
            norm_v = 1 / norm_v;

            q_[3] *= norm_v;

            q_[0] *= norm_v;
            q_[1] *= norm_v;
            q_[2] *= norm_v;
        }
    }

    Quaternion normalized() const {
        auto copy = *this;

        copy.normalize();
        return copy;
    }

    friend Quaternion operator+(const Quaternion &q1, const Quaternion &q2) {
        return {q1.getVectorPart() + q2.getVectorPart(), q1.getScalarPart() + q2.getScalarPart()};
    }

    friend Quaternion operator*(const Quaternion &q1, const Quaternion &q2) {
        auto vec1 = q1.getVectorPart();
        auto vec2 = q2.getVectorPart();

        auto scal1 = q1.getScalarPart();
        auto scal2 = q2.getScalarPart();

        auto scalar = scal1 * scal2 - vec1 * vec2;
        auto vec = scal1 * vec2 + scal2 * vec1 + Quaternion::VectorType::cross(vec1, vec2);

        return {vec, scalar};
    }

    Quaternion conjugate() const {
        auto scalar = w();
        auto vec = getVectorPart() * -1;
        return {vec, scalar};
    }

    Quaternion inverse() const {
        auto norm_val = norm();
        norm_val *= norm_val;
        norm_val = 1 / norm_val;

        auto conj = conjugate();

        auto scalar = conj.w() * norm_val;
        auto tmp = getVectorPart();
        auto vec = tmp * norm_val;

        return {vec, scalar};
    }

private:

    StorageType q_;
};

using Quaterniond = Quaternion<core::EngineScalar>;
}// namespace rcbe::math

#endif