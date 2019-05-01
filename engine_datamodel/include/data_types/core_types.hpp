#ifndef RCBE_CORE_TYPES_H
#define RCBE_CORE_TYPES_H

#include <string>
#include <vector>
#include <array>
#include <algorithm>

namespace rcbe::data_types::core
{

template <typename ValueType>
class Vector
{
public:

    using value_type = ValueType;

public:
    Vector(ValueType &&x, ValueType &&y, ValueType &&z)
    :
    _x(x)
    , _y(y)
    , _z(z)
    {}
    ~Vector() = default;

    Vector(const Vector &other) = default;
    Vector &operator=(const Vector &other) = default;

    Vector(Vector &&other) = default;
    Vector &operator=(Vector &&other) = default;

private:
    ValueType _x;
    ValueType _y;
    ValueType _z;
};

using Vector3d = Vector<double>;

template <typename ValueType, size_t DimRow, size_t DimCol>
class Matrix
{
public:
    using value_type = ValueType;
    /*const size_t rows = DimRow;
    const size_t columns = DimCol;*/

public:
    Matrix(const std::vector<ValueType> &m)
    {
        const auto total = DimRow * DimCol;
        if(m.size() != DimRow * DimCol)
            throw std::runtime_error("Wrong number of arguments for matrix initialization!");

        std::copy(m.begin(), m.end(), _m.begin());
    }
    ~Matrix() = default;

    Matrix(const Matrix &other) = default;
    Matrix &operator=(const Matrix &other) = default;

    Matrix(Matrix &&other) = default;
    Matrix &operator=(Matrix &&other) = default;

private:
    std::array<ValueType, DimRow * DimCol> _m;
};

using Matrix3x3d = Matrix<double, 3, 3>;
}

#endif //RCBE_CORE_TYPES_H