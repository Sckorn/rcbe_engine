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

template <typename ValueType, size_t dim, template <typename ValT, size_t n> typename StorageBase = std::array>
class Vector
{
    static_assert(std::is_scalar_v<ValueType>);
    static_assert(dim == 2 || dim == 3); // Vectors of only 2 or 3 dimensional space
public:

    static constexpr size_t dimension = dim;

public:

    using value_type = ValueType;
    using storage_type = StorageBase<value_type, dimension>;

public:

    Vector()
    {
        _v.fill(0);
    };

    ~Vector() = default;

    template <typename... valt>
    Vector(valt&&... args)
    :
    _v( {args... })
    {}

    Vector(const Vector &other) = default;
    Vector &operator=(const Vector &other) = default;

    Vector(Vector &&other) = default;
    Vector &operator=(Vector &&other) = default;

    value_type &operator[](const size_t index)
    {
        return _v[index];
    }

    value_type x()
    {
        return _v[0];
    }

    value_type y()
    {
        return _v[1];
    }

    template <typename = std::enable_if_t<dim == 3>>
    value_type z()
    {
        return _v[2];
    }

    value_type length() const
    {
        size_t sum = 0;
        for (const auto &e : _v)
        {
            sum += e * e;
        }

        return sum;
    }

    Vector normalized()
    {
        const auto len = length();

        auto copy = *this;
        copy.normalize();

        return copy;
    }

    void normalize()
    {
        const auto len = length();
        for (auto &e : _v)
        {
            e /= len;
        }
    }

private:
    StorageBase<ValueType, dim> _v;
};

}

#endif