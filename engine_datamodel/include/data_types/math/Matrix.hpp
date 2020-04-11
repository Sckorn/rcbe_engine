#ifndef RCBE_MATRIX_HPP
#define RCBE_MATRIX_HPP

#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <iostream>

#include <data_types/config/data_model_config.hpp>
#include <data_types/math/Vector.hpp>
#include <data_types/math/Quaternion.hpp>

#include <common/fuzzy_logic/fuzzy_logic.hpp>

#include <nlohmann/json.hpp>

namespace rcbe::math
{
template <typename ValueType, size_t DimRow, size_t DimCol>
class Matrix
{
public:
    using value_type = ValueType;
    using storage_type = std::array<value_type, DimRow * DimCol>;
    using column_type = rcbe::math::Vector<value_type, DimRow>;

    static const size_t rows = DimRow;
    static const size_t columns = DimCol;

    static_assert(std::is_scalar_v<ValueType>);

public:
    Matrix()
    {
        for (size_t row = 0; row < DimRow; ++row)
        {
            for  (size_t col = 0; col < DimCol; ++col)
            {
                auto index = row * DimRow + col;
                if (row == col)
                {
                    _m[index] = 1;
                }
                else
                {
                    _m[index] = 0;
                }
            }
        }
    }

    template <typename T = void, typename = std::enable_if_t<(DimRow == DimCol) && (DimCol == 3), T> >
    Matrix(const Quaternion<value_type> &quat)
    {
        auto s = quat.norm();
        auto square_x = quat.x() * quat.x();
        auto square_y = quat.y() * quat.y();
        auto square_z = quat.z() * quat.z();

        auto x = quat.x();
        auto y = quat.y();
        auto z = quat.z();
        auto w = quat.w();

        _m[0] = 1 - 2 * s * (square_y + square_z);
        _m[1] = 2 * s * (x * y - w * z);
        _m[2] = 2 * s * (w * y + x * z);

        _m[3] = 2 * s * (x * y + w * z);
        _m[4] = 1 - 2 * s * (square_x + square_z);
        _m[5] = 2 * (y * z - w * x);

        _m[6] = 2 * s * (x * z - w * y);
        _m[7] = 2 * s * (w * x + y * z);
        _m[8] = 1 - 2 * s * (square_x + square_y);
    }

    template <typename ... Args, typename = std::enable_if_t< (std::is_convertible_v<Args, value_type> && ...) && sizeof...(Args) == DimCol * DimRow > >
    Matrix(Args&&... values)
    :
    _m { value_type(values)... }
    {
        
    }

    Matrix(const std::vector<ValueType> &m)
    {
        const auto total = DimRow * DimCol;
        if(m.size() != DimRow * DimCol)
            throw std::runtime_error("Wrong number of arguments for matrix initialization!");

        std::copy(m.begin(), m.end(), _m.begin());
    }

    template <typename InputIterator>
    Matrix(InputIterator begin, InputIterator end)
    {
        if (begin == end)
            throw std::runtime_error("Empty iterators range!");

        if (std::distance(begin, end) != DimCol * DimRow)
            throw std::runtime_error("Iterators range has wrong size");

        for (InputIterator it = begin; it != end; ++it)
        {
            _m[std::distance(begin, it)] = *it;
        }
    }

    ~Matrix() = default;

    Matrix(const Matrix &other) = default;
    Matrix &operator=(const Matrix &other) = default;

    Matrix(Matrix &&other) = default;
    Matrix &operator=(Matrix &&other) = default;

    // required for basis manipulations
    column_type getColumn(size_t index) const
    {
        if (index >= DimCol)
            throw std::out_of_range("Can't get column of matrix");

        std::array<value_type, DimRow> _tmp;
        for (size_t i = 0; i < DimRow; ++i)
        {
            _tmp[i] = _m[i * DimCol + index];
        }

        return column_type {_tmp};
    }

    // row-major
    const value_type &at(size_t row, size_t col) const
    {
        return _m.at(row * DimRow + col);
    }

    Matrix transpose() const
    {
        storage_type transposed_source {};
        for (size_t i = 0; i < rows; ++i)
        {
            const auto col = getColumn(i);
            for (size_t j = 0; j < col.dimension; ++j)
            {
                transposed_source[i * DimRow + j] = col.at(j);
            }
        }

        return Matrix(std::make_move_iterator(transposed_source.begin()), std::make_move_iterator(transposed_source.end()));
    }

    const storage_type &data() const
    {
        return _m;
    }

    static Matrix &identity()
    {
        return Matrix {};
    } 

private:

    storage_type _m;
};

using Matrix3x3 = Matrix<rcbe::core::EngineScalar, 3, 3>;
using Matrix4x4 = Matrix<rcbe::core::EngineScalar, 4, 4>;
}

namespace nlohmann
{
template <>
struct adl_serializer<rcbe::math::Matrix3x3> 
{
    static void to_json(nlohmann::json& j, const rcbe::math::Matrix3x3& value);
    static void from_json(const nlohmann::json& j, rcbe::math::Matrix3x3& value);
};
}

rcbe::math::Matrix3x3 operator*(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Matrix3x3 &rhs);
rcbe::math::Matrix3x3 operator+(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Matrix3x3 &rhs);
rcbe::math::Vector3d operator*(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Vector3d &rhs);
rcbe::math::Matrix4x4 operator*(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Matrix4x4 &rhs);
rcbe::math::Matrix4x4 operator+(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Matrix4x4 &rhs);
rcbe::math::Vector4d operator*(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Vector4d &rhs);

#endif