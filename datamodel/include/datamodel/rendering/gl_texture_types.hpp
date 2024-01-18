#ifndef RCBE_ENGINE_GL_TEXTURE_TYPES_HPP
#define RCBE_ENGINE_GL_TEXTURE_TYPES_HPP

#include <GL/gl.h>

#include <rcbe-engine/datamodel/visual/texture_types.hpp>

namespace rdmn::render {
enum class TextureWrappingType {
    repeat = GL_REPEAT,
    mirrored_repeat = GL_MIRRORED_REPEAT,
    clamp_to_edge = GL_CLAMP_TO_EDGE,
    clamp_to_border = GL_CLAMP_TO_BORDER
};

enum class TextureType {
    texture_1d = GL_TEXTURE_1D,
    texture_2d = GL_TEXTURE_2D,
    texture_3d = GL_TEXTURE_3D
};

enum class TextureFilteringType {
    nearest = GL_NEAREST,
    linear = GL_LINEAR,
    nearest_mipmap_nearest = GL_NEAREST_MIPMAP_NEAREST,
    linear_mipmap_nearest = GL_LINEAR_MIPMAP_NEAREST,
    nearest_mipmap_linear = GL_NEAREST_MIPMAP_LINEAR,
    linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR
};

struct rasterizer_texture_config {
    TextureWrappingType wrapping_type = TextureWrappingType::repeat;
    TextureType texture_type = TextureType::texture_2d;
    TextureFilteringType filtering_type = TextureFilteringType::linear;
    rcbe::visual::texture_config image_config;
};
}// namespace rdmn::render

#endif//RCBE_ENGINE_GL_TEXTURE_TYPES_HPP
