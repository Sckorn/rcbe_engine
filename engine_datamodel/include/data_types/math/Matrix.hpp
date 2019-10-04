#ifndef RCBE_MATRIX_HPP
#define RCBE_MATRIX_HPP

#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <iostream>

#include <data_types/core/ArrayBase.hpp>
#include <data_types/data_model_config.hpp>

namespace rcbe::math
{
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
}

#endif