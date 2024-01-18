#include <cstddef>
#include <string>

#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include <rcbe-engine/fundamentals/convinience.hpp>

inline constexpr size_t BITS_IN_BYTE = 8;

namespace rcbe::visual {

Texture::Texture(rdmn::vis::image_data &&data)
    : data_ {
          rdmn::vis::image_data {
              .metadata = {
                  .origin = {data.metadata.origin},
                  .dimensions = {data.metadata.dimensions},
                  .pixel_depth_bytes = data.metadata.pixel_depth_bytes,
                  .component_order = data.metadata.component_order,
              },
              .pixels = data.pixels->shared_from_this(),// TODO: dubious solution, think of a better one! @sckorn
          }} {}

Texture::Texture(const core::EnginePath &path, ParserType &&parser)
    : data_ {
          parser(path)} {}

Texture::~Texture() = default;

size_t Texture::getWidth() const {
    return data_.metadata.dimensions.width;
}

size_t Texture::getHeight() const {
    return data_.metadata.dimensions.height;
}

size_t Texture::getBitsPerPixel() const {
    return data_.metadata.pixel_depth_bytes * BITS_IN_BYTE;
}

size_t Texture::getBytesPerPixel() const {
    return data_.metadata.pixel_depth_bytes;
}

size_t Texture::getImageSizeBytes() const {
    return sizeof(typename decltype(data_)::PixelsType::ValueType) * getWidth() * getHeight();
}

const texture_config::ImageBodyType &Texture::getPixels() const {
    return *(data_.pixels);
}

rdmn::vis::image_data Texture::cloneImageData() const {
    rdmn::vis::image_data r {
        .metadata = data_.metadata,
        .pixels = data_.pixels->shared_from_this(),
    };

    return r;
}

TexturePtr make_tex_ptr(const core::EnginePath &path, Texture::ParserType &&parser) {
    return std::make_shared<Texture>(path, std::move(parser));
}

TextureConstPtr make_tex_const_ptr(const core::EnginePath &path, Texture::ParserType &&parser) {
    return std::make_shared<const Texture>(path, std::move(parser));
}

VisualTextureSet::VisualTextureSet(std::initializer_list<TexturePtr> &&tex_data)
    : grouped_textures(tex_data) {
}

VisualTextureSet::VisualTextureSet(std::unordered_set<TexturePtr> &&tex_data)
    : grouped_textures(std::move(tex_data)) {
}

}// namespace rcbe::visual