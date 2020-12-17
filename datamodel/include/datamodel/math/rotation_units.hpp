#ifndef RCBE_ENGINE_ROTATION_UNITS_HPP
#define RCBE_ENGINE_ROTATION_UNITS_HPP

#include <cmath>
#include <variant>
#include <ostream>

#include <rcbe-engine/utils/math_utils.hpp>

namespace rcbe::math {

namespace detail {
template <typename Actual>
struct rotations_base {
    explicit rotations_base(double rot)
    :
    rotation_(rot)
    {}

    explicit operator double() const {
        return rotation_;
    }

    rotations_base& operator+=(rotations_base d) {
        rotation_ += d.rotation_;
        return *this;
    }

    rotations_base& operator-=(rotations_base d) {
        rotation_ -= d.rotation_;
        return *this;
    }

    rotations_base& operator+=(Actual a) {
        const auto& base = static_cast<const rotations_base&>(a);
        return this->operator+=(base);
    }

    rotations_base& operator-=(Actual a) {
        const auto& base = static_cast<const rotations_base&>(a);
        return this->operator+=(base);
    }

    friend std::ostream& operator<<(std::ostream& os, const rotations_base& d) {
        os << d.rotation_;
        return os;
    }

    friend auto operator<=>(const rotations_base& l, const rotations_base& r) = default;

protected:
    double rotation_;
};
}

struct deg : public detail::rotations_base<deg> {
    explicit deg(double rot)
    :
    detail::rotations_base<deg>( rot)
    {
        while (rotation_ > 360.0) {
            rotation_ -= 360;
        }

        while (rotation_ < -360.) {
            rotation_ += 360.;
        }
    }
};

struct rad : public detail::rotations_base<rad> {
    explicit rad(double rot )
    :
    detail::rotations_base<rad>(rot)
    {
        while (rotation_ > 2 * M_PI) {
            rotation_ -= 2 * M_PI;
        }

        while (rotation_ < -2 * M_PI) {
            rotation_ += 2 * M_PI;
        }
    }
};

}

#endif //RCBE_ENGINE_ROTATION_UNITS_HPP
