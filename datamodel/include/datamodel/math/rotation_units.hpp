#ifndef RCBE_ENGINE_ROTATION_UNITS_HPP
#define RCBE_ENGINE_ROTATION_UNITS_HPP

#include <cmath>
#include <ostream>
#include <variant>

#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/utils/math_utils.hpp>
#include <rdmn-engine/public_api.hpp>

namespace rcbe::math {

namespace detail {
template <typename Actual>
struct rotations_base {

    explicit rotations_base(double rot)
        : rotation_(rot) {}

    explicit operator double() const {
        return rotation_;
    }

    explicit operator float() const {
        return static_cast<float>(rotation_);
    }

    rotations_base &operator+=(rotations_base d) {
        rotation_ += d.rotation_;
        return *this;
    }

    rotations_base &operator-=(rotations_base d) {
        rotation_ -= d.rotation_;
        return *this;
    }

    rotations_base &operator+=(Actual a) {
        const auto &base = static_cast<const rotations_base &>(a);
        return this->operator+=(base);
    }

    rotations_base &operator-=(Actual a) {
        const auto &base = static_cast<const rotations_base &>(a);
        return this->operator-=(base);
    }

    friend rotations_base operator-(rotations_base l, rotations_base r) {
        return rotations_base(l.rotation_ - r.rotation_);
    }

    friend std::ostream &operator<<(std::ostream &os, const rotations_base &d) {
        os << d.rotation_;
        return os;
    }

    friend auto operator<=>(const rotations_base &l, const rotations_base &r) = default;

protected:

    void clamp(const double clamp_value) {
        while (rotation_ > clamp_value) {
            rotation_ -= clamp_value;
        }

        while (rotation_ < -clamp_value) {
            rotation_ += clamp_value;
        }
    }

    double rotation_;
};
}// namespace detail

struct deg;// need forward declaration only for rad but leaving two for consistency (?)
struct rad;

struct R_PUBLIC_API deg : public detail::rotations_base<deg> {
    static constexpr double ABS_MAX_VALUE = 360.;

     explicit deg(double rot);

    [[nodiscard]] rad as_rad() const;
};

struct R_PUBLIC_API rad : public detail::rotations_base<rad> {
    static constexpr double ABS_MAX_VALUE = 2 * M_PI;

    explicit rad(double rot);

    [[nodiscard]] deg as_deg() const;
};

}// namespace rcbe::math

#endif//RCBE_ENGINE_ROTATION_UNITS_HPP
