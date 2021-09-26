#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include "TGATexture.hpp"

namespace rcbe::visual {

Texture::Texture(
        const core::EnginePath &path,
        texture_config config,
        bool for_rasterizer)
:
impl_(std::make_unique<TextureImplementation>(path, config, for_rasterizer)) {

}

Texture::~Texture() = default;
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