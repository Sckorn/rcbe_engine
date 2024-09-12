#ifndef RCBE_ENGINE_TEXTURE_HPP
#define RCBE_ENGINE_TEXTURE_HPP

#include <functional>
#include <memory>
#include <unordered_set>

#include <rcbe-engine/datamodel/math/DynamicMatrix.hpp>
#include <rcbe-engine/datamodel/visual/image_data.hpp>
#include <rcbe-engine/datamodel/visual/texture_types.hpp>
#include <rcbe-engine/fundamentals/types.hpp>

#include <rdmn-engine/public_api.hpp>
namespace rcbe::visual {

class TextureImplementation;

R_PUBLIC_API class Texture : std::enable_shared_from_this<Texture> {
public:

    using ParserType = std::function<rdmn::vis::image_data(const core::EnginePath &)>;

    R_PUBLIC_API Texture() = delete;
    /// TODO: should it store the parser? @sckorn
    R_PUBLIC_API explicit Texture(rdmn::vis::image_data &&data);
    R_PUBLIC_API Texture(const core::EnginePath &path, ParserType &&parser);
    R_PUBLIC_API ~Texture();

    R_PUBLIC_API Texture(Texture &&other) noexcept = default;
    R_PUBLIC_API Texture &operator=(Texture &&other) noexcept = default;

    [[nodiscard]] R_PUBLIC_API size_t getWidth() const;
    [[nodiscard]] R_PUBLIC_API size_t getHeight() const;
    [[nodiscard]] R_PUBLIC_API size_t getBitsPerPixel() const;
    [[nodiscard]] R_PUBLIC_API size_t getBytesPerPixel() const;
    [[nodiscard]] R_PUBLIC_API size_t getImageSizeBytes() const;
    [[nodiscard]] R_PUBLIC_API const texture_config::ImageBodyType &getPixels() const;
    [[nodiscard]] R_PUBLIC_API rdmn::vis::image_data cloneImageData() const;

private:

    rdmn::vis::image_data data_;
};

using TexturePtr = std::shared_ptr<Texture>;
using TextureConstPtr = std::shared_ptr<const Texture>;

R_PUBLIC_API TexturePtr make_tex_ptr(const core::EnginePath &path, Texture::ParserType &&parser);
R_PUBLIC_API TextureConstPtr make_tex_const_ptr(const core::EnginePath &path, Texture::ParserType &&parser);

R_PUBLIC_API struct VisualTextureSet {
    R_PUBLIC_API VisualTextureSet(std::initializer_list<TexturePtr> &&tex_data);
    R_PUBLIC_API explicit VisualTextureSet(std::unordered_set<TexturePtr> &&tex_data);

    R_PUBLIC_API ~VisualTextureSet() = default;

    const std::unordered_set<TexturePtr> grouped_textures;
};

}// namespace rcbe::visual

#endif//RCBE_ENGINE_TEXTURE_HPP
