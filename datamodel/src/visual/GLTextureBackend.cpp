#include <rcbe-engine/core/gl_extensions.hpp>
#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

#include "GLTextureBackend.hpp"

namespace rcbe::visual {
TextureRasterizerBackendImpl::TextureRasterizerBackendImpl(
        const uint8_t * texture_raw_data,
        const texture_config &config,
        const size_t width,
        const size_t height
        )
:
config_(config) {
    glGenTextures(1, reinterpret_cast<unsigned int *>(&id_));
    bind();

    glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_WRAP_S, static_cast<GLenum>(config_.wrapping_type));
    glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_WRAP_T, static_cast<GLenum>(config_.wrapping_type));

    if (config_.texture_type == GLTextureType::texture_3d) {
        glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_WRAP_R, static_cast<GLenum>(config_.wrapping_type));
    }

    if (config_.wrapping_type == GLTextureWrappingType::clamp_to_border) {
        RGBAColor color {0., 0., 0., 1.};
        auto color_as_array = color.asArray();
        glTexParameterfv(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_BORDER_COLOR, reinterpret_cast<const GLfloat *>(color_as_array.data()));
    }

    glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(config_.filtering_type));
    glTexParameteri(static_cast<GLenum>(config_.texture_type), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(config_.filtering_type));

    glTexImage2D(static_cast<GLenum>(config_.texture_type),
                 0, GL_RGBA,
                 width, height,
                 0, GL_RGBA,  GL_UNSIGNED_BYTE, texture_raw_data);
    glGenerateMipmap(static_cast<GLenum>(config_.texture_type));

    unbind();
}

void TextureRasterizerBackendImpl::bind(const size_t index) const {
    if (index >= GL_MAX_TEXTURE_UNITS)
        throw std::runtime_error("Can't activate texture" + std::to_string(index));

    glActiveTexture(static_cast<size_t>(GL_TEXTURE0) + index);
    glBindTexture(static_cast<GLenum>(config_.texture_type), id_);
}

void TextureRasterizerBackendImpl::unbind() const {
    glBindTexture(static_cast<GLenum>(config_.texture_type), 0);
}

}