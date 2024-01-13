#include "GLRasterizerTexture.hpp"

#include <GL/gl.h>

#include <rcbe-engine/core/gl_extensions.hpp>

#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

namespace rdmn::render {
RasterizerTextureImplementation::RasterizerTextureImplementation(
        rasterizer_texture_config config,
        rcbe::visual::TexturePtr texture
)
:
config_(config) {
    glGenTextures(1, reinterpret_cast<unsigned int *>(&id_));
    bind();

    glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_WRAP_S, static_cast<GLenum>(config_.wrapping_type));
    glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_WRAP_T, static_cast<GLenum>(config_.wrapping_type));

    if (config_.texture_type == TextureType::texture_3d) {
        glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_WRAP_R, static_cast<GLenum>(config_.wrapping_type));
    }

    if (config_.wrapping_type == TextureWrappingType::clamp_to_border) {
        rcbe::visual::RGBAColor color {0., 0., 0., 1.};
        auto color_as_array = color.asArray();
        glTexParameterfv(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_BORDER_COLOR, reinterpret_cast<const GLfloat *>(color_as_array.data()));
    }

    glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(config_.filtering_type));
    glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(config_.filtering_type));

    glTexImage2D(static_cast<GLenum>(config_.texture_type),
                 0, GL_RGBA,
                 texture->getWidth(), texture->getHeight(),
                 0, GL_RGBA,  GL_UNSIGNED_BYTE, reinterpret_cast<const uint8_t*>(texture->getPixels().rawData()));
    glGenerateMipmap(static_cast<GLenum>(config_.texture_type));

    unbind();
}

void RasterizerTextureImplementation::bind(const size_t index) const {
    if (index >= GL_MAX_TEXTURE_UNITS)
        throw std::runtime_error("Can't activate texture" + std::to_string(index));

    glActiveTexture(static_cast<size_t>(GL_TEXTURE0) + index);
    glBindTexture(static_cast<GLenum>(config_.texture_type), id_);
}

void RasterizerTextureImplementation::unbind() const {
    glBindTexture(static_cast<GLenum>(config_.texture_type), 0);
}

bool RasterizerTextureImplementation::deferred() const noexcept {
    return deferred_;
}
}
