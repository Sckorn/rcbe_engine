#ifndef RCBE_MATRIX_HPP
#define RCBE_MATRIX_HPP

#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <iostream>

#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/math/Quaternion.hpp>

#include <rcbe-engine/fuzzy_logic/fuzzy_logic.hpp>
#include <rcbe-engine/utils/math_utils.hpp>

#include <nlohmann/json.hpp>

#ifdef RDMN_GLM_INTEGRATION
#include <glm/mat4x4.hpp>
#endif

namespace rcbe::math
{
template <typename Value, size_t DimRow, size_t DimCol>
class Matrix
{
public:
    using ValueType = Value;
    using StorageType = std::array<ValueType, DimRow * DimCol>;
    using ColumnType = rcbe::math::Vector<ValueType, DimRow>;
    using RowType = rcbe::math::Vector<ValueType, DimCol>;

    static constexpr size_t ROWS = DimRow;
    static constexpr size_t COLUMNS = DimCol;

#ifdef RCBE_SCALAR_LINALG
    static_assert(std::is_scalar_v<ValueType>, "Current build supports only scalar matrices!");
#endif

public:
    constexpr Matrix()
    {
        for (size_t row = 0; row < ROWS; ++row)
        {
            for  (size_t col = 0; col < COLUMNS; ++col)
            {
                auto index = row * ROWS + col;
                if (row == col)
                {
                    m_[index] = 1;
                }
                else
                {
                    m_[index] = 0;
                }
            }
        }
    }

    template <typename T, typename = std::enable_if_t<(DimRow == DimCol) && (DimCol == 3) && std::is_convertible_v<T, ValueType>, void> >
    explicit Matrix(const Quaternion<T> &quat)
    {
        auto s = quat.norm();
        auto square_x = quat.x() * quat.x();
        auto square_y = quat.y() * quat.y();
        auto square_z = quat.z() * quat.z();

        auto x = quat.x();
        auto y = quat.y();
        auto z = quat.z();
        auto w = quat.w();

        m_[0] = 1 - 2 * s * (square_y + square_z);
        m_[1] = 2 * s * (x * y - w * z);
        m_[2] = 2 * s * (w * y + x * z);

        m_[3] = 2 * s * (x * y + w * z);
        m_[4] = 1 - 2 * s * (square_x + square_z);
        m_[5] = 2 * (y * z - w * x);

        m_[6] = 2 * s * (x * z - w * y);
        m_[7] = 2 * s * (w * x + y * z);
        m_[8] = 1 - 2 * s * (square_x + square_y);
    }

    template <typename ... Args, typename = std::enable_if_t<(std::is_convertible_v<Args, ValueType> && ...) && sizeof...(Args) == DimCol * DimRow > >
    Matrix(Args&&... values)
    :
    m_ {ValueType(values)... }
    {
        
    }

    template <typename V, typename = std::enable_if_t<std::is_convertible_v<V, ValueType>, void>>
    explicit Matrix(const std::vector<V> &m)
    {
        const auto total = ROWS * COLUMNS;
        if(m.size() != total)
            throw std::runtime_error("Wrong number of arguments for matrix initialization!");

        std::copy(m.begin(), m.end(), m_.begin());
    }

    template <typename V, typename = std::enable_if_t<std::is_convertible_v<V, ValueType>, void>>
    explicit Matrix(V(&source)[ROWS * COLUMNS])
    {
        for (size_t i = 0; i < ROWS * COLUMNS; ++i) {
            m_[i] = source[i];
        }
    }

    template <typename InputIterator>
    Matrix(InputIterator begin, InputIterator end)
    {
        if (begin == end)
            throw std::runtime_error("Empty iterators range!");

        if (std::distance(begin, end) != COLUMNS * ROWS)
            throw std::runtime_error("Iterators range has wrong size");

        for (InputIterator it = begin; it != end; ++it)
        {
            m_[std::distance(begin, it)] = *it;
        }
    }

#ifdef RDMN_GLM_INTEGRATION
    template <
            glm::length_t rows,
            glm::length_t cols,
            typename T,
            glm::qualifier Q,
            typename = std::enable_if_t<
                    std::is_convertible_v<T, ValueType>
                            && ROWS == COLUMNS
                            && rows == cols
                            && ROWS == rows
                            && COLUMNS == cols
                            && (ROWS == 4 || ROWS == 3),
                    void >
    >
    Matrix(glm::mat<cols, rows, T, Q> &&source) {
        for (size_t i = 0; i < ROWS; ++i) {
            const auto source_column = source[i];
            for (size_t j = 0; j < COLUMNS; ++j) {
                m_[i * ROWS + j] = source_column[j];
            }
        }
    }

    template <
            glm::length_t rows,
            glm::length_t cols,
            typename T,
            glm::qualifier Q,
            typename = std::enable_if_t<
                    std::is_convertible_v<T, ValueType>
                            && ROWS == COLUMNS
                            && rows == cols
                            && ROWS == rows
                            && COLUMNS == cols
                            && (ROWS == 4 || ROWS == 3),
                    void >
    >
    explicit operator glm::mat<cols, rows, T, Q>() const {
        const auto tansposed_copy = this->transposed();
        glm::mat<cols, rows, T, Q> ret{1.};
        for (size_t i = 0; i < cols; ++i) {
            for (size_t j = 0; j < rows; ++j) {
                ret[i][j] = tansposed_copy.at(j, i);
            }
        }

        return ret;
    }
#endif

    ~Matrix() = default;

    Matrix(const Matrix &other) = default;
    Matrix &operator=(const Matrix &other) = default;

    Matrix(Matrix &&other) noexcept = default;
    Matrix &operator=(Matrix &&other) noexcept = default;

    // required for basis manipulations
    /// TODO: redo it so it returns special struct, that holds const references to row elements @sckorn
    [[nodiscard]] ColumnType getColumn(size_t index) const
    {
        if (index >= COLUMNS)
            throw std::out_of_range("Can't get column of matrix");

        std::array<ValueType, ROWS> _tmp;
        for (size_t i = 0; i < ROWS; ++i)
        {
            _tmp[i] = m_[i * COLUMNS + index];
        }

        return ColumnType {_tmp};
    }

    [[nodiscard]] RowType getRow(size_t index) const {
        if (index >= ROWS)
            throw std::runtime_error("Can't get row of matrix");

        std::array<ValueType, COLUMNS> _tmp;
        for (size_t i = 0; i < COLUMNS; ++i)
        {
            _tmp[i] = m_[i + ROWS * index];
        }

        return RowType {_tmp};
    }

    // row-major
    const ValueType &at(size_t row, size_t col) const
    {
        return m_.at(row * ROWS + col);
    }

    Matrix transposed() const
    {
        StorageType transposed_source {};
        for (size_t i = 0; i < ROWS; ++i)
        {
            const auto col = getColumn(i);
            for (size_t j = 0; j < col.DIMENSION; ++j)
            {
                transposed_source[i * ROWS + j] = col.at(j);
            }
        }

        return Matrix(std::make_move_iterator(transposed_source.begin()), std::make_move_iterator(transposed_source.end()));
    }

    // TODO: consider a std::array implementation if memory usage is too excessive
    template <typename T = void, typename = std::enable_if_t<ROWS == COLUMNS, T>>
    [[nodiscard]] ValueType determinant() const {
        std::vector<ValueType> param;
        param.reserve(m_.size());
        for (const auto& m : m_) {
            param.push_back(m);
        }
        return determinant(param, ROWS, COLUMNS);
    }

    Matrix inversed() const {
        auto det = determinant();
        if (det == 0) {
            throw std::runtime_error("Can't inverse matrix, determinant is zero.");
        }

        std::vector<ValueType> intermediate_storage {m_.begin(), m_.end() };
        std::vector<ValueType > minors_matrix;

        auto storage_size = m_.size();

        minors_matrix.reserve(storage_size);

        for (size_t i = 0; i < storage_size; ++i) {
            const auto c_row = i / ROWS;
            const auto c_col = i % COLUMNS;

            auto cof = cofactorCompute(intermediate_storage, ROWS, COLUMNS, c_row, c_col);
            minors_matrix.push_back(determinant(cof, ROWS - 1, COLUMNS - 1));
        }

        auto alg_additions = algebraicAdditions(minors_matrix);

        auto alg_trn = Matrix(alg_additions.begin(), alg_additions.end()).transposed();

        det = 1 / det;

        return det * alg_trn;
    }

    const StorageType &data() const
    {
        return m_;
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, ValueType>, void>>
    explicit operator Matrix<U, ROWS, COLUMNS>() {
        return convertUnderlyingValues<U>();
    }

    template<typename V, typename = std::enable_if_t<std::is_convertible_v<ValueType, V>, void>>
    Matrix<V, ROWS, COLUMNS> convertUnderlyingValues() const {
        V storage[ROWS * COLUMNS];
        for (size_t i = 0; i < m_.size(); ++i) {
            storage[i] = static_cast<V>(m_[i]);
        }

        return Matrix<V, ROWS, COLUMNS>{storage};
    }

    static Matrix &identity()
    {
        return Matrix {};
    }

    friend Matrix operator*(const Matrix& m, const ValueType scalar) {
        Matrix copy = m;
        for (auto& c : copy.m_) {
            c *= scalar;
        }

        return copy;
    }

    friend Matrix operator*(const ValueType scalar, const Matrix& m) {
        return m * scalar;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (const auto& mm : m.m_) {
            os << mm << " ";
        }

        return os;
    }

private:

    std::vector<ValueType> algebraicAdditions(const std::vector<ValueType >& m) const {
        std::vector<ValueType> ret;
        ret.reserve(m.size());
        for (size_t i = 0; i < m_.size(); ++i) {
            const auto curr_row = i / ROWS;
            const auto curr_col = i % COLUMNS;

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

    std::vector<ValueType> cofactorCompute(const std::vector<ValueType >& m, const size_t size_r, const size_t size_c, const size_t exclude_row, const size_t exclude_col) const {
        std::vector<ValueType > ret;
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

    ValueType determinant(const std::vector<ValueType>& m, size_t dim_rows, size_t dim_cols) const {
        if (m.size() == 1)
            return m[0];

        ValueType det = 0;

        int mul_ratio = 1;

        for (size_t i = 0; i < dim_cols; ++i) {
            const auto v = m[i];

            auto cof = cofactorCompute(m, dim_rows, dim_cols, 0, i);

            det += mul_ratio * v * determinant(cof, dim_rows - 1, dim_cols - 1);

            mul_ratio = -mul_ratio;
        }

        return det;
    }

    StorageType m_;
};

// TODO: rename the two below, adding a d (for double) suffix
using Matrix3x3 = Matrix<rcbe::core::EngineScalar, 3, 3>;
using Matrix4x4 = Matrix<rcbe::core::EngineScalar, 4, 4>;
using Matrix4x4f = Matrix<float, 4, 4>;
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

/// TODO: make all ops template @sckorn
rcbe::math::Matrix3x3 operator*(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Matrix3x3 &rhs);
rcbe::math::Matrix3x3 operator+(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Matrix3x3 &rhs);

rcbe::math::Vector3d operator*(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Vector3d &rhs);

template <typename V>
rcbe::math::Matrix<V, 4, 4> operator*(const rcbe::math::Matrix<V, 4, 4> &lhs, const rcbe::math::Matrix<V, 4, 4> &rhs) {
    std::vector<typename rcbe::math::Matrix<V, 3, 3>::ValueType> interm;
    interm.reserve(lhs.ROWS * lhs.COLUMNS);
    for (size_t i = 0; i < lhs.ROWS * lhs.COLUMNS; ++i)
    {
        const size_t row_index = i / lhs.ROWS;
        const size_t sub_index = i % lhs.COLUMNS;

        const auto r_column = rhs.getColumn(sub_index);

        interm.push_back(
                lhs.at(row_index, 0) * r_column.x() +
                lhs.at(row_index, 1) * r_column.y() +
                lhs.at(row_index, 2) * r_column.z() +
                lhs.at(row_index, 3) * r_column.w()
                );
    }

    return rcbe::math::Matrix<V, 4, 4> {interm};
}
rcbe::math::Matrix4x4 operator+(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Matrix4x4 &rhs);

rcbe::math::Vector4d operator*(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Vector4d &rhs);

#endif