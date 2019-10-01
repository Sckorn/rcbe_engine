#ifndef RCBE_VECTOR_HPP
#define RCBE_VECTOR_HPP

#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>
#include <cmath>

#include <iostream>

#include <data_types/core/ArrayBase.hpp>
#include <data_types/data_model_config.hpp>

namespace rcbe::geometry
{

template <size_t dim, typename ValueType = rcbe::data_model::EngineScalar, template <size_t n, typename ValT> typename StorageBase = data_model::ArrayBase>
class Vector
{
    static_assert(std::is_scalar_v<ValueType>);
public:

    using _value_type = ValueType;

    static constexpr size_t dimension = dim;

public:

    Vector() = default;
    ~Vector() = default;

    Vector(ValueType x, ValueType y, ValueType z)
    :
    _v(x, y, z)
    {}

    Vector(const Vector &other) = default;
    Vector &operator=(const Vector &other) = default;

    Vector(Vector &&other) = default;
    Vector &operator=(Vector &&other) = default;

    const ValueType &operator[](const size_t index) const
    {
        if (index > dimension)
            throw std::runtime_error("Access for wrong dimension");
        return _v[index];
    }

    _value_type x() const
    {
        return _v[0];
    }

    _value_type y() const
    {
        return _v[1];
    }

    _value_type z() const
    {
        return _v[2];
    }

    _value_type length() const
    {
        return std::sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
    }

    Vector normalized()
    {
        const auto len = length();
        return ValueType { _v[0] / len, _v[1] / len, _v[2] / len };
    }

    //void norm

private:
    StorageBase<dim, ValueType> _v;
};

}

#endif