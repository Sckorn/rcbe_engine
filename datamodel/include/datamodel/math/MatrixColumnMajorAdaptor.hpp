#ifndef RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP
#define RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP

#include <array>
#include <cstdint>

#include <rcbe-engine/datamodel/math/Matrix.hpp>

#ifdef RCBE_GLM_INTEGRATION
#include <glm/matrix.hpp>
#endif

namespace rcbe::math {
template <typename StoredValue = core::EngineScalar>
class MatrixColumnMajorAdaptor {
public:
    using ValueType = StoredValue;
    using StorageType = std::vector<ValueType>;

    MatrixColumnMajorAdaptor() = default;
    ~MatrixColumnMajorAdaptor() = default;

#ifdef RCBE_GLM_INTEGRATION
    template <glm::length_t r, glm::length_t c, typename T, glm::qualifier Q>
    explicit MatrixColumnMajorAdaptor(const glm::mat<c, r, T, Q> &matrix)
    {
        adapted_storage_.reserve(r * c);
        for (size_t i = 0; i < c; ++i) {
            for (size_t j = 0; j < r; ++j) {
                adapted_storage_.push_back(static_cast<ValueType>(matrix[i][j]));
            }
        }
    }
#endif

#ifdef RCBE_DEBUG_MODE
    template <typename Value, size_t sz>
    explicit MatrixColumnMajorAdaptor(std::array<Value, sz> storage)
    {
        for (const auto &s : storage) {
            adapted_storage_.push_back(s);
        }
    }
#endif


    /// A constructor that allows build from vector that stores columns sequentially
    explicit MatrixColumnMajorAdaptor(StorageType &&storage)
    :
    adapted_storage_(std::move(storage))
    {}

    template <typename value, size_t r, size_t c>
    explicit MatrixColumnMajorAdaptor(const Matrix<value, r, c>& m)
    :
    adapted_storage_(r * c, 0)
    {
        for (size_t j = 0; j < c; ++j) {
            const auto column = m.getColumn(j);
            for (size_t i = 0; i < column.DIMENSION; ++i) {
                adapted_storage_[j * c + i] = column.at(i);
            }
        }
    }

    [[nodiscard]] const StorageType ::value_type * getRaw() const {
        return adapted_storage_.data();
    }

    [[nodiscard]] size_t size() const {
        return adapted_storage_.size();
    }

    template <typename NewValueType>
    [[nodiscard]] MatrixColumnMajorAdaptor<NewValueType> convertValuesTo() const {
        typename MatrixColumnMajorAdaptor<NewValueType>::StorageType storage;
        storage.reserve(adapted_storage_.size());
        for (const auto &a : adapted_storage_) {
            storage.push_back(static_cast<NewValueType>(a));
        }

        return MatrixColumnMajorAdaptor<NewValueType>(std::move(storage));
    }

#ifdef RCBE_DEBUG_MODE
    friend std::ostream &operator<<(std::ostream& os, const MatrixColumnMajorAdaptor &a) {
        for (const auto& s : a.adapted_storage_) {
            os << s << " ";
        }
        return os;
    }
#endif

private:

     StorageType adapted_storage_;
};
}

#endif //RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP
