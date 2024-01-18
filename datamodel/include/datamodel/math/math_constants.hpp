#ifndef RCBE_MATH_HPP
#define RCBE_MATH_HPP

#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/fundamentals/types.hpp>

namespace rcbe::math {
#ifdef RDMN_VULKAN
static constexpr Vector3d WORLD_UP(0., 0., 1.);
#elif defined(RDMN_OPENGL)
static constexpr Vector3d WORLD_UP(0., 1., 0.);
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
#else
inline constexpr Vector3d WORLD_UP(0., 0., 1.);// consider clarifying whether it should be this or (0., -1., -0.);
inline constexpr Vector3d WORLD_FORWARD(1., 0., 0.);
inline constexpr Vector3d WORLD_RIGHT(0.0, -1., 0.);
#endif
}// namespace rcbe::math

#endif