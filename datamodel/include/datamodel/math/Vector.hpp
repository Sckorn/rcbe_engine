#ifndef RCBE_VECTOR_HPP
#define RCBE_VECTOR_HPP

#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>
#include <cmath>

#include <iostream>

#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/fundamentals/concepts.hpp>

// TODO: replace the two below by corresponding fwd header (introduce one for binary)

#include <rcbe-engine/binary/BinaryBuffer.hpp>

#include <nlohmann/json.hpp>



namespace rcbe::math
{

template <typename Value, size_t dim, template <typename valt, size_t n> typename StorageBase = std::array>
class Vector
{
#ifdef RCBE_SCALAR_LINALG
    static_assert(std::is_scalar_v<Value>, "Vector can only be built using scalar values");
#endif
    static_assert(dim > 1 && dim < 5); // 4d vector is base for quaternion or RGBA color
public:

    static constexpr size_t DIMENSION = dim;

    using ValueType = Value;
    using StorageType = StorageBase<ValueType, DIMENSION>;

public:

    constexpr Vector() {
        v_.fill(0);
    };

    Vector(const StorageType &s)
    :
    v_(s)
    {}

    ~Vector() = default;

    template <typename ValT, typename = std::enable_if_t<(std::is_convertible_v<ValT, ValueType>), void> >
    Vector(const std::array<ValT, DIMENSION> &a) {
        for (size_t i = 0; i < DIMENSION; ++i) {
            v_[i] = a[i];
        }
    }

    template <typename ValT, size_t S, typename = std::enable_if_t<std::is_convertible_v<ValT, ValueType> && S == DIMENSION>>
    Vector(const Vector<ValT, S> &source) {
        for (size_t i = 0; i < DIMENSION; ++i) {
            v_[i] = static_cast<ValueType>(source.at(i));
        }
    }

    template <typename... valt, typename = std::enable_if_t<(std::is_convertible_v<valt, ValueType> && ... ) && (sizeof...(valt) == DIMENSION) > >
    constexpr Vector(valt&&... args)
    :
    v_({static_cast<ValueType>(args)...} )
    {}

    Vector(const Vector &other) = default;
    Vector &operator=(const Vector &other) = default;

    Vector(Vector &&other) = default;
    Vector &operator=(Vector &&other) = default;

    auto rawData() const {
        return v_.data();
    }

    ValueType &operator[](const size_t index)
    {
        return v_[index];
    }

    const ValueType &operator[](const size_t index) const
    {
        return v_[index];
    }

    const ValueType &at(const size_t index) const
    {
        return v_.at(index);
    }

    ValueType &x()
    {
        return v_[0];
    }

    const ValueType &x() const
    {
        return v_[0];
    }

    ValueType &y()
    {
        return v_[1];
    }

    const ValueType &y() const
    {
        return v_[1];
    }

    template <typename T = void, typename = std::enable_if_t< (dim > 2), T > >
    ValueType &z()
    {
        return v_[2];
    }

    template <typename T = void, typename = std::enable_if_t< (dim > 2), T > >
    const ValueType &z() const
    {
        return v_[2];
    }

    template < typename T = void, typename = std::enable_if_t< (dim == 4), T > >
    ValueType &w()
    {
        return v_[3];
    }

    template < typename T = void, typename = typename std::enable_if_t< (dim == 4), T > >
    const ValueType &w() const
    {
        return v_[3];
    }

    template < typename T = void, typename = typename std::enable_if_t< (dim < 4), T > >
    ValueType length() const
    {
        size_t sum = 0;
        for (const auto &e : v_)
        {
            sum += e * e;
        }

        return std::sqrt(sum);
    }

    template < typename T = void, typename = typename std::enable_if_t< (dim < 4), T > >
    Vector normalized() const
    {
        auto copy = *this;
        copy.normalize();

        return copy;
    }

    template < typename T = void, typename = typename std::enable_if_t< (dim < 4), T > >
    void normalize()
    {
        const auto len = length();
        if (len != 0) {
            for (auto &e : v_)
            {
                e /= len;
            }
        }
    }

    [[nodiscard]] size_t dimension() const {
        return DIMENSION;
    }

    static ValueType dot(const Vector& v1, const Vector& v2)
    {
        return v1 * v2;
    }

    void operator+=(const Vector &a)
    {
        for(size_t i = 0; i < DIMENSION; ++i)
        {
            v_[i] += a.v_[i];
        }
    }

    template < typename T = void, typename = typename std::enable_if_t< (dim == 3), T > >
    static Vector cross(const Vector& v1, const Vector& v2)
    {
        return { v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x()  };
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector &vec) {
        os << "(";
        for (size_t i = 0; i < DIMENSION; ++i) {
            os << vec.at(i);
            if (i != DIMENSION - 1)
                os << ", ";
        }
        os << ")";
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Vector &vec) {
        Vector::StorageType st;
        for (size_t i = 0; i < DIMENSION; ++i) {
            is >> st[i];
        }
        vec = Vector(st);
        return is;
    }

    template <typename TargetType, typename = std::enable_if_t<std::is_convertible_v<ValueType, TargetType>, void>>
    [[nodiscard]] Vector<TargetType, DIMENSION> convertUnderlyingType() const {
        StorageBase<TargetType, DIMENSION> tmp;
        for (size_t i = 0; i < DIMENSION; ++i) {
            tmp[i] = static_cast<TargetType>(v_[i]);
        }
        return Vector<TargetType, DIMENSION> {
            tmp
        };
    }


private:
    StorageBase<ValueType, dim> v_;
};

using Vector4d = Vector<core::EngineScalar, 4>;
using Vector3d = Vector<core::EngineScalar, 3>;
using Vector2d = Vector<core::EngineScalar, 2>;
using Vector4f = Vector<float, 4>;
using Vector3f = Vector<float, 3>;
using Vector2f = Vector<float, 2>;
}

namespace nlohmann
{
template <>
struct adl_serializer<rcbe::math::Vector3d>
{
static void to_json(nlohmann::json &j, const rcbe::math::Vector3d &v);
static void from_json(const nlohmann::json &j, rcbe::math::Vector3d &v);
};

template <>
struct adl_serializer<rcbe::math::Vector2d>
{
static void to_json(nlohmann::json &j, const rcbe::math::Vector2d &v);
static void from_json(const nlohmann::json &j, rcbe::math::Vector2d &v);
};
}

namespace rcbe::binary {
template <>
void from_binary(const BinaryBuffer &b, rcbe::math::Vector3f &v);
template <>
void to_binary(BinaryBuffer &b, const rcbe::math::Vector3f &v);
}

template <typename V, size_t D>
typename rcbe::math::Vector<V, D>::ValueType operator*(const rcbe::math::Vector<V, D> &lhs, const rcbe::math::Vector<V, D> &rhs) {
    typename rcbe::math::Vector<V, D>::ValueType ret {};

    for (size_t i = 0; i < lhs.dimension(); ++i) {
        ret += lhs.at(i) * rhs.at(i);
    }

    return ret;
}

template <typename V, size_t D>
rcbe::math::Vector<V, D> operator+(const rcbe::math::Vector<V, D> &lhs, const rcbe::math::Vector<V, D> &rhs) {
    rcbe::math::Vector<V, D> ret{};

    for (size_t i = 0; i < ret.dimension(); ++i) {
        ret[i] = lhs[i] + rhs[i];
    }

    return ret;
}

template <typename V, size_t D>
rcbe::math::Vector3d operator-(const rcbe::math::Vector<V, D> &lhs, const rcbe::math::Vector<V, D> &rhs) {
    rcbe::math::Vector<V, D> ret{};

    for (size_t i = 0; i < ret.dimension(); ++i) {
        ret[i] = lhs[i] - rhs[i];
    }

    return ret;
}

template <rdmn::core::NumberValue NV, typename V, size_t D>
rcbe::math::Vector<V, D> operator*(const rcbe::math::Vector<V, D> &lhs, NV &&rhs) {
    rcbe::math::Vector<V, D> ret{};

    for (size_t i = 0; i < ret.dimension(); ++i) {
        ret[i] = lhs[i] * static_cast<V>(rhs);
    }

    return ret;
}

template <rdmn::core::NumberValue NV, typename V, size_t D>
rcbe::math::Vector<V, D> operator*(NV &&lhs, const rcbe::math::Vector<V, D> &rhs) {
    return rhs * lhs;
}

template <typename V, size_t D>
rcbe::math::Vector<V, D> operator-(const rcbe::math::Vector<V, D> &v) {
    return v * static_cast<typename rcbe::math::Vector<V, D>::ValueType>(-1);
}


#endif