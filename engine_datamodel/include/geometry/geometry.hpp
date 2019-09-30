#ifndef RCBE_GEOMETRY_HPP
#define RCBE_GEOMETRY_HPP

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <iostream>

namespace rcbe::geometry
{

template <typename ValueType, size_t dim>
class Vector
{
public:

    using value_type = ValueType;

    static const size_t dimension = dim;

public:

    Vector() 
    {
    };


    // fancy stuff below, but useless, will generate a lot of unneeded code
    /*template <typename... Args>
    Vector(Args&&... args)
    :
    _v {{args...}}
    {
        constexpr size_t size = sizeof...(Args);
        static_assert(size == dim);
    }*/

    ~Vector() 
    {
        static_assert(std::is_scalar_v<ValueType>);
    };

    Vector(const Vector &other) = default;
    Vector &operator=(const Vector &other) = default;

    Vector(Vector &&other) = default;
    Vector &operator=(Vector &&other) = default;

    ValueType operator[](const size_t index) const
    {
        if (index > dimension)
            throw std::runtime_error("Acces for wrong dimension");
        return _v[index];
    }

private:
    std::array<ValueType, dim> _v;
};

using Vector3d = Vector<double, 3>;
using Vector2d = Vector<double, 2>;

template <typename ValueType, size_t DimRow, size_t DimCol>
class Matrix
{
public:
    using value_type = ValueType;
    static const size_t rows = DimRow;
    static const size_t columns = DimCol;

public:
    Matrix(const std::vector<ValueType> &m)
    {
        const auto total = DimRow * DimCol;
        if(m.size() != DimRow * DimCol)
            throw std::runtime_error("Wrong number of arguments for matrix initialization!");

        std::copy(m.begin(), m.end(), _m.begin());
    }
    ~Matrix()
    {
        static_assert(std::is_scalar_v<ValueType>);
    };

    Matrix(const Matrix &other) = default;
    Matrix &operator=(const Matrix &other) = default;

    Matrix(Matrix &&other) = default;
    Matrix &operator=(Matrix &&other) = default;

private:
    std::array<ValueType, DimRow * DimCol> _m;
};

using Matrix3x3d = Matrix<double, 3, 3>;
}

#endif