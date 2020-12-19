#ifndef RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP
#define RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP

#include <cstdint>

#include <rcbe-engine/datamodel/math/Matrix.hpp>

namespace rcbe::math {
class MatrixColumnMajorAdaptor {
public:
    using StorageType = std::vector<core::EngineScalar>;

    MatrixColumnMajorAdaptor() = default;
    ~MatrixColumnMajorAdaptor() = default;

    template <typename value, size_t r, size_t c>
    explicit MatrixColumnMajorAdaptor(const Matrix<value, r, c>& m)
    :
    adapted_storage_(r * c, 0)
    {
        using matrix_type = Matrix<value, r, c>;

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

#ifdef RCBE_DEBUG_MODE
    friend std::ostream& operator<<(std::ostream& os, const rcbe::math::MatrixColumnMajorAdaptor& a) {
        for (const auto& s : a.adapted_storage_) {
            os << s << " ";
        }
        os << std::endl;
        return os;
    }
#endif

private:
     StorageType adapted_storage_;
};
}

#endif //RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP
