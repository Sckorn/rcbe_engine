#ifndef RCBE_ENGINE_DYNAMICMATRIX_HPP
#define RCBE_ENGINE_DYNAMICMATRIX_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include <rcbe-engine/fundamentals/types.hpp>

namespace rcbe::math {
template <typename Value>
class DynamicMatrix : public std::enable_shared_from_this<DynamicMatrix<Value>> {
public:
    using ValueType = Value;
    using StorageType = std::vector<ValueType>;
    using RawValueType = const ValueType *;

    DynamicMatrix()
    :
    rows_ { 0 }
    , cols_ { 0 } {}
    ~DynamicMatrix() = default;

    DynamicMatrix(const size_t rows, const size_t columns)
    :
    rows_ { rows }
    , cols_ { columns } {
        storage_.reserve(rows * columns);
    }

    DynamicMatrix(const DynamicMatrix &other) = delete;
    DynamicMatrix &operator=(const DynamicMatrix &other) = delete;

    DynamicMatrix(DynamicMatrix &&other) noexcept = default;
    DynamicMatrix &operator=(DynamicMatrix &&other) noexcept = default;

    [[nodiscard]] RawValueType rawData() const {
        return storage_.data();
    }

    [[nodiscard]] ValueType at(const size_t row, const size_t col) const {
        return storage_.at(row * cols_ + col);
    }

    void pushBack(const ValueType &v) {
        storage_.push_back(v);
    }

    void pushBack(ValueType &&v) {
        storage_.push_back(std::move(v));
    }

    [[nodiscard]] size_t size() const {
        return storage_.size();
    }

#ifdef RCBE_DEBUG_MODE
    ValueType &at(const size_t index) {
        return storage_[index];
    }
#endif
    /// TODO: the below solution to copying is dubios, in the very least,
    /// maybe it's more practical, to just enable copy ctor, think about it @sckorn
    DynamicMatrix clone() const {
        return DynamicMatrix(rows_, cols_, storage_);
    }

private:

    DynamicMatrix(const size_t rows, const size_t cols, StorageType storage)
    :
    rows_ {rows}
    , cols_ {cols}
    , storage_ {storage} {}

    size_t rows_;
    size_t cols_;
    StorageType storage_;
};

template <typename V>
using DynamicMatrixPtr = std::shared_ptr<DynamicMatrix<V>>;
template <typename V>
using ConstDynamicMatrixPtr = std::shared_ptr<const DynamicMatrix<V>>;
}

#endif //RCBE_ENGINE_DYNAMICMATRIX_HPP
