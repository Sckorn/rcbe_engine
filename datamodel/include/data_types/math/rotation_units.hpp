#ifndef RCBE_ENGINE_ROTATION_UNITS_HPP
#define RCBE_ENGINE_ROTATION_UNITS_HPP

#include <cmath>
#include <variant>
#include <ostream>

#include <common/utils/math_utils.hpp>

namespace rcbe::math {

// TODO: consider using float for degs and rads

struct deg {
    explicit deg(double rot)
    :
    rotation_( rot)
    {
        while (rotation_ > 360.0) {
            rotation_ -= 360;
        }

        while (rotation_ < -360.) {
            rotation_ += 360.;
        }
    }

    explicit operator double() const {
        return rotation_;
    }

    deg& operator+=(deg d) {
        rotation_ += d.rotation_;
        return *this;
    }

    deg& operator-=(deg d) {
        rotation_ -= d.rotation_;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const deg& d) {
        os << d.rotation_;
        return os;
    }

private:
    double rotation_;
};

struct rad {
    explicit rad(double rot )
    :
    rotation_(rot)
    {
        while (rotation_ > 2 * M_PI) {
            rotation_ -= 2 * M_PI;
        }

        while (rotation_ < -2 * M_PI) {
            rotation_ += 2 * M_PI;
        }
    }

    explicit operator double() const {
        return rotation_;
    }

    rad& operator+=(rad r) {
        rotation_ += r.rotation_;
        return *this;
    }

    rad& operator-=(rad r) {
        rotation_ -= r.rotation_;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const rad& r) {
        os << r.rotation_;
        return os;
    }
private:
    double rotation_;
};

}

#endif //RCBE_ENGINE_ROTATION_UNITS_HPP
