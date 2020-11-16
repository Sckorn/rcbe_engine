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
#include <common/utils/math_utils.hpp>

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

    static constexpr size_t rows = DimRow;
    static constexpr size_t columns = DimCol;

    static_assert(std::is_scalar_v<ValueType>);

public:
    Matrix()
    {
        for (size_t row = 0; row < rows; ++row)
        {
            for  (size_t col = 0; col < columns; ++col)
            {
                auto index = row * rows + col;
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

    template <typename V, typename = std::enable_if_t<std::is_convertible_v<V, value_type>, void>>
    explicit Matrix(const std::vector<V> &m)
    {
        const auto total = rows * columns;
        if(m.size() != total)
            throw std::runtime_error("Wrong number of arguments for matrix initialization!");

        std::copy(m.begin(), m.end(), _m.begin());
    }

    template <typename V, typename = std::enable_if_t<std::is_convertible_v<V, value_type>, void>>
    explicit Matrix(V(&source)[rows * columns])
    {
        for (size_t i = 0; i < rows * columns; ++i) {
            _m[i] = source[i];
        }
    }

    template <typename InputIterator>
    Matrix(InputIterator begin, InputIterator end)
    {
        if (begin == end)
            throw std::runtime_error("Empty iterators range!");

        if (std::distance(begin, end) != columns * rows)
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
        if (index >= columns)
            throw std::out_of_range("Can't get column of matrix");

        std::array<value_type, rows> _tmp;
        for (size_t i = 0; i < rows; ++i)
        {
            _tmp[i] = _m[i * columns + index];
        }

        return column_type {_tmp};
    }

    // row-major
    const value_type &at(size_t row, size_t col) const
    {
        return _m.at(row * rows + col);
    }

    Matrix transposed() const
    {
        storage_type transposed_source {};
        for (size_t i = 0; i < rows; ++i)
        {
            const auto col = getColumn(i);
            for (size_t j = 0; j < col.dimension; ++j)
            {
                transposed_source[i * rows + j] = col.at(j);
            }
        }

        return Matrix(std::make_move_iterator(transposed_source.begin()), std::make_move_iterator(transposed_source.end()));
    }

    // TODO: consider a std::array implementation if memory usage is too excessive
    template <typename T = void, typename = std::enable_if_t<rows == columns, T>>
    [[nodiscard]] value_type determinant() const {
        std::vector<value_type> param;
        param.reserve(_m.size());
        for (const auto& m : _m) {
            param.push_back(m);
        }
        return determinant(param, rows, columns);
    }

    Matrix inversed() const {
        auto det = determinant();
        if (det == 0) {
            throw std::runtime_error("Can't inverse matrix, determinant is zero.");
        }

        std::vector<value_type> intermediate_storage { _m.begin(), _m.end() };
        std::vector<value_type > minors_matrix;

        auto storage_size = _m.size();

        minors_matrix.reserve(storage_size);

        for (size_t i = 0; i < storage_size; ++i) {
            const auto c_row = i / rows;
            const auto c_col = i % columns;

            auto cof = cofactor_compute(intermediate_storage, rows, columns, c_row, c_col);
            minors_matrix.push_back(determinant(cof, rows - 1, columns - 1));
        }

        auto alg_additions = algebraic_additions(minors_matrix);

        auto alg_trn = Matrix(alg_additions.begin(), alg_additions.end()).transposed();

        det = 1 / det;

        return det * alg_trn;
    }

    const storage_type &data() const
    {
        return _m;
    }

    static Matrix &identity()
    {
        return Matrix {};
    }

    friend Matrix operator*(const Matrix& m, const value_type scalar) {
        Matrix copy = m;
        for (auto& c : copy._m) {
            c *= scalar;
        }

        return copy;
    }

    friend Matrix operator*(const value_type scalar, const Matrix& m) {
        return m * scalar;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (const auto& mm : m._m) {
            os << mm << " ";
        }
        os << std::endl;
        return os;
    }

private:

    std::vector<value_type> algebraic_additions(const std::vector<value_type >& m) const {
        std::vector<value_type> ret;
        ret.reserve(m.size());
        for (size_t i = 0; i < _m.size(); ++i) {
            const auto curr_row = i / rows;
            const auto curr_col = i % columns;

            if(rcbe::math::even(curr_row) && rcbe::math::odd(curr_col)) {
                ret.push_back(-m[i]);
            } else if (rcbe::math::even(curr_col) && rcbe::math::odd(curr_row)) {
                ret.push_back(-m[i]);
            } else {
                ret.push_back(m[i]);
            }
        }

        return ret;
    }

    std::vector<value_type> cofactor_compute(const std::vector<value_type >& m, const size_t size_r, const size_t size_c, const size_t exclude_row, const size_t exclude_col) const {
        std::vector<value_type > ret;
        ret.reserve((size_r - 1) * (size_c - 1));
        for (size_t i = 0; i < size_r * size_c; ++i) {
            const auto row_internal = i / size_r;
            const auto col_internal = i % size_c;

            if (row_internal != exclude_row && col_internal != exclude_col) {
                ret.push_back(m[i]);
            }
        }

        return ret;
    }

    value_type determinant(const std::vector<value_type>& m, size_t dim_rows, size_t dim_cols) const {
        if (m.size() == 1)
            return m[0];

        value_type det = 0;

        int mul_ratio = 1;

        for (size_t i = 0; i < dim_cols; ++i) {
            const auto v = m[i];

            auto cof = cofactor_compute(m, dim_rows, dim_cols, 0, i);

            det += mul_ratio * v * determinant(cof, dim_rows - 1, dim_cols - 1);

            mul_ratio = -mul_ratio;
        }

        return det;
    }

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