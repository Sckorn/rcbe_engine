#ifndef RCBE_ENGINE_RASTERIZER_TEXTURE_TYPES_HPP
#define RCBE_ENGINE_RASTERIZER_TEXTURE_TYPES_HPP

#include <rcbe-engine/fundamentals/convinience.hpp>

#if defined(RDMN_VULKAN)
#include "vk_texture_types.hpp"
#elif defined(RDMN_OPENGL)
#include "gl_texture_types.hpp"
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
// TODO: introduce one for DirectX
#else
static_assert(false, rdmn::core::RASTERIZER_NOT_SET_ERROR_MSG);
#endif

#endif //RCBE_ENGINE_RASTERIZER_TEXTURE_TYPES_HPP
