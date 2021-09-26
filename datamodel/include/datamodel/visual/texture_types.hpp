#ifndef RCBE_ENGINE_TEXTURE_TYPES_HPP
#define RCBE_ENGINE_TEXTURE_TYPES_HPP

#include <GL/gl.h>

namespace rcbe::visual {
// TODO: remove GL from this name, when more renderers are intorduced
enum class GLTextureWrappingType {
    repeat = GL_REPEAT,
    mirrored_repeat = GL_MIRRORED_REPEAT,
    clamp_to_edge = GL_CLAMP_TO_EDGE,
    clamp_to_border = GL_CLAMP_TO_BORDER
};

// TODO: remove GL from this name, when more renderers are intorduced
enum class GLTextureType {
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

enum class TextureComponentOrder {
    RGB,
    RGBA,
    GBR,
    GBRA,
    ARGB
};

struct texture_config {
    GLTextureWrappingType wrapping_type = GLTextureWrappingType::repeat;
    GLTextureType texture_type = GLTextureType::texture_2d;
    TextureFilteringType filtering_type = TextureFilteringType::linear;
    TextureComponentOrder component_order = TextureComponentOrder::RGBA;
};

}

#endif //RCBE_ENGINE_TEXTURE_TYPES_HPP
