#ifndef RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP
#define RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP

#include <cstdint>

#include <data_types/math/Matrix.hpp>

namespace rcbe::math {
class MatrixColumnMajorAdaptor {
public:
    using storage_type = std::vector<core::EngineScalar>;

    MatrixColumnMajorAdaptor() = default;
    ~MatrixColumnMajorAdaptor() = default;

    template <typename value, size_t r, size_t c>
    explicit MatrixColumnMajorAdaptor(const Matrix<value, r, c>& m)
    :
    adapted_storage_(r * c, 0)
    {
        using matrix_type = Matrix<value, r, c>;
        static_assert(r * c == matrix_type::rows * matrix_type::columns, "Dimensions of matrix and adaptor should be equal");

        for (size_t j = 0; j < c; ++j) {
            const auto column = m.getColumn(j);
            for (size_t i = 0; i < column.dimension; ++i) {
                adapted_storage_[j * c + i] = column.at(i);
            }
        }
    }

    [[nodiscard]] const storage_type ::value_type * get_raw() const {
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
     storage_type adapted_storage_; // TODO: common type, value wrapper or something else ?
};
}

#endif //RCBE_ENGINE_MATRIXCOLUMNMAJORADAPTOR_HPP
