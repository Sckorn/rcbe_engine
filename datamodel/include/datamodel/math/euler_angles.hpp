#ifndef RCBE_ENGINE_EULER_ANGLES_HPP
#define RCBE_ENGINE_EULER_ANGLES_HPP

#include <rcbe-engine/datamodel/math/rotation_units.hpp>

namespace rcbe::math {
namespace detail {

template <typename Actual>
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
            return std::get<rad>(rotation_).as_deg();
        }

        return std::get<deg>(rotation_);
    }

    [[nodiscard]] rad as_rad() const {
        if (rotation_.index() == 1) {
            return std::get<deg>(rotation_).as_rad();
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

    euler_base& operator+=(const euler_base& b) {
        return this->operator+=(b.as_deg());
    }

    euler_base& operator+=(const Actual& a) {
        const auto& base = static_cast<const euler_base&>(a);

        return this->operator+=(base);
    }

    friend std::ostream& operator<<(std::ostream& os, const euler_base& eb) {
        if (eb.rotation_.index() == 0) {
            os << static_cast<const double>(std::get<rad>(eb.rotation_));
        } else {
            os << static_cast<const double>(std::get<deg>(eb.rotation_));
        }
        return os;
    }

    friend auto operator<=>(const euler_base& l, const euler_base& r) {
        return l.as_deg() <=> r.as_deg();
    }

private:
    std::variant<rad, deg> rotation_;
};
}

struct roll : public detail::euler_base<roll>{
    using euler_base::euler_base;
};

struct yaw : public detail::euler_base<yaw> {
    using euler_base::euler_base;
};

struct pitch : public detail::euler_base<pitch> {
    using euler_base::euler_base;
};
}

#endif //RCBE_ENGINE_EULER_ANGLES_HPP
