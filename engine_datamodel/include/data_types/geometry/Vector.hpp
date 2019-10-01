#ifndef RCBE_VECTOR_HPP
#define RCBE_VECTOR_HPP

#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

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

    Vector(const Vector &other) = default;
    Vector &operator=(const Vector &other) = default;

    Vector(Vector &&other) = default;
    Vector &operator=(Vector &&other) = default;

    const ValueType &operator[](const size_t index) const
    {
        if (index > dimension)
            throw std::runtime_error("Acces for wrong dimension");
        return _v[index];
    }

private:
    StorageBase<dim, ValueType> _v;
};

}

#endif