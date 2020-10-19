#ifndef RCBE_ENGINE_EULER_ANGLES_HPP
#define RCBE_ENGINE_EULER_ANGLES_HPP

#include <data_types/math/rotation_units.hpp>

namespace rcbe::math {
namespace detail {

// TODO: implement all basic operations
// leave comparisons for after c++20 upgrade (need that spaceship operator)
struct euler_base {
    explicit euler_base(rad r)
            :
            rotation_(r) {}

    explicit euler_base(deg d)
            :
            rotation_(d) {}

    virtual ~euler_base() = default;

    [[nodiscard]] deg as_deg() const {
        if (rotation_.index() == 0) {
            return deg(rad_to_deg(static_cast<const core::EngineScalar >(std::get<rad>(rotation_))));
        }

        return std::get<deg>(rotation_);
    }

    [[nodiscard]] rad as_rad() const {
        if (rotation_.index() == 1) {
            return rad(deg_to_rad(static_cast<const core::EngineScalar>(std::get<deg>(rotation_))));
        }

        return std::get<rad>(rotation_);
    }

    // TODO: implement clamping
    euler_base& operator+=(const deg& addition) {
        if (rotation_.index() == 0) {
            auto& r = std::get<rad>(rotation_);
            r += rad(deg_to_rad(static_cast<const core::EngineScalar>(addition)));
        } else {
            auto& r = std::get<deg>(rotation_);
            r += addition;
        }

        return *this;
    }

    euler_base& operator+=(const rad& addition) {
        if (rotation_.index() == 0) {
            auto& r = std::get<rad>(rotation_);
            r += addition;
        } else {
            auto& r = std::get<deg>(rotation_);
            r += deg(rad_to_deg(static_cast<const core::EngineScalar>(addition)));
        }

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const euler_base& eb) {
        if (eb.rotation_.index() == 0) {
            os << static_cast<const double>(std::get<rad>(eb.rotation_));
        } else {
            os << static_cast<const double>(std::get<deg>(eb.rotation_));
        }
        return os;
    }

private:
    std::variant<rad, deg> rotation_;
};
}

struct yaw : protected detail::euler_base {
    using euler_base::euler_base;
    using euler_base::as_deg;
    using euler_base::as_rad;
    using euler_base::operator+=;

    yaw& operator+=(const yaw& y) {
        auto& actual = dynamic_cast<euler_base&>(*this);
        actual += y.as_deg();
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const yaw& y) {
        const auto& actual = dynamic_cast<const euler_base&>(y);
        os << actual;
        return os;
    }
};

struct pitch : protected detail::euler_base {
    using euler_base::euler_base;
    using euler_base::as_deg;
    using euler_base::as_rad;
    using euler_base::operator+=;

    friend std::ostream& operator<<(std::ostream& os, const pitch& y) {
        const auto& actual = dynamic_cast<const euler_base&>(y);
        os << actual;
        return os;
    }

    pitch& operator+=(const pitch& p) {
        auto& actual = dynamic_cast<euler_base&>(*this);
        actual += p.as_deg();
        return *this;
    }
};
}

#endif //RCBE_ENGINE_EULER_ANGLES_HPP
