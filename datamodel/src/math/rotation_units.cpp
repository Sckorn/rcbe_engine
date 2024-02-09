#include <rcbe-engine/datamodel/math/rotation_units.hpp>

namespace rcbe::math {
deg::deg(double rot)
    : detail::rotations_base<deg>(rot) {
    clamp(deg::ABS_MAX_VALUE);
}

rad::rad(double rot)
    : detail::rotations_base<rad>(rot) {
    clamp(rad::ABS_MAX_VALUE);
}

rad deg::as_rad() const {
    return rad(deg_to_rad(static_cast<const core::EngineScalar>(rotation_)));
}

deg rad::as_deg() const {
    return deg(rad_to_deg(static_cast<const core::EngineScalar>(rotation_)));
}
}// namespace rcbe::math