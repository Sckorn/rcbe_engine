#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include "TGATexture.hpp"

namespace rcbe::visual {

Texture::Texture(
        const core::EnginePath &path,
        texture_config config,
        bool for_rasterizer)
:
impl_(std::make_unique<TextureImplementation>(path, config, for_rasterizer)) {}

Texture::Texture(const Texture &other)
:
impl_(
        std::make_unique<TextureImplementation>(
                other.impl_->getTexturePath(),
                other.impl_->getTextureConfig(),
                other.impl_->forRasterizer()
        )
) {

}

Texture &Texture::operator=(const Texture &other) {
    if (this == &other)
        return *this;

    impl_ = std::make_unique<TextureImplementation>(
            other.impl_->getTexturePath(), other.impl_->getTextureConfig(), other.impl_->forRasterizer()
    );

    return *this;
}

Texture &Texture::operator=(Texture &&other) = default;

Texture::~Texture() = default;

Texture::Texture(Texture &&other) = default;

size_t Texture::getWidth() const {
    return impl_->getWidth();
}

size_t Texture::getHeight() const {
    return impl_->getHeight();
}

size_t Texture::getBitsPerPixel() const {
    return impl_->getBitsPerPixel();
}

const Texture::ImageBodyType &Texture::getImageBody() const {
    return impl_->getBody();
}

void Texture::bind(const size_t index) const {
    impl_->bind(index);
}

void Texture::unbind() const {
    impl_->unbind();
}

}