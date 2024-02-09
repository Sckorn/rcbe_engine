#ifndef RCBE_ENGINE_TEXTURE_HPP
#define RCBE_ENGINE_TEXTURE_HPP

#include <functional>
#include <memory>
#include <unordered_set>

#include <rcbe-engine/datamodel/math/DynamicMatrix.hpp>
#include <rcbe-engine/datamodel/visual/image_data.hpp>
#include <rcbe-engine/datamodel/visual/texture_types.hpp>
#include <rcbe-engine/fundamentals/types.hpp>

namespace rcbe::visual {

class TextureImplementation;

class Texture : std::enable_shared_from_this<Texture> {
public:

    using ParserType = std::function<rdmn::vis::image_data(const core::EnginePath &)>;

    Texture() = delete;
    /// TODO: should it store the parser? @sckorn
    explicit Texture(rdmn::vis::image_data &&data);
    Texture(const core::EnginePath &path, ParserType &&parser);
    ~Texture();

    Texture(Texture &&other) noexcept = default;
    Texture &operator=(Texture &&other) noexcept = default;

    [[nodiscard]] size_t getWidth() const;
    [[nodiscard]] size_t getHeight() const;
    [[nodiscard]] size_t getBitsPerPixel() const;
    [[nodiscard]] size_t getBytesPerPixel() const;
    [[nodiscard]] size_t getImageSizeBytes() const;
    [[nodiscard]] const texture_config::ImageBodyType &getPixels() const;
    [[nodiscard]] rdmn::vis::image_data cloneImageData() const;

private:

    rdmn::vis::image_data data_;
};

using TexturePtr = std::shared_ptr<Texture>;
using TextureConstPtr = std::shared_ptr<const Texture>;

TexturePtr make_tex_ptr(const core::EnginePath &path, Texture::ParserType &&parser);
TextureConstPtr make_tex_const_ptr(const core::EnginePath &path, Texture::ParserType &&parser);

struct VisualTextureSet {
    VisualTextureSet(std::initializer_list<TexturePtr> &&tex_data);
    explicit VisualTextureSet(std::unordered_set<TexturePtr> &&tex_data);

    ~VisualTextureSet() = default;

    const std::unordered_set<TexturePtr> grouped_textures;
};

}// namespace rcbe::visual

#endif//RCBE_ENGINE_TEXTURE_HPP
