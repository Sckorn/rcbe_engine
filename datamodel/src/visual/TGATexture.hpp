#ifndef RCBE_ENGINE_TGATEXTURE_HPP
#define RCBE_ENGINE_TGATEXTURE_HPP

#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/datamodel/math/DynamicMatrix.hpp>
#include <rcbe-engine/binary/BinaryBuffer.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include <rcbe-engine/datamodel/visual/texture_types.hpp>

#include "GLTextureBackend.hpp"

namespace rcbe::visual {
class TextureImplementation {
public:

    struct ColorMapSpecification {
        static constexpr size_t SIZE = sizeof(uint16_t) * 2 + sizeof(uint8_t);

        uint16_t first_entry_index;
        uint16_t color_map_length;
        uint8_t color_map_entry_size;
    };

    struct ImageSpecification {
        static  constexpr size_t SIZE = sizeof(uint16_t) * 4 + sizeof(uint8_t) * 2;

        uint16_t x_origin;
        uint16_t y_origin;
        uint16_t width;
        uint16_t height;
        uint8_t pixel_depth;
        uint8_t image_descriptor;
    };

    using ImageBodyType = math::DynamicMatrix<uint32_t>;

    TextureImplementation() = delete;
    TextureImplementation(const core::EnginePath &path, texture_config config, bool for_rasterizer = false);
    ~TextureImplementation() = default;

    [[nodiscard]] const ImageBodyType &getBody() const;
    [[nodiscard]] const ColorMapSpecification &getColorMapSpec() const;
    [[nodiscard]] const ImageSpecification &getImageSpec() const;

    [[nodiscard]] size_t getWidth() const;
    [[nodiscard]] size_t getHeight() const;
    [[nodiscard]] size_t getBitsPerPixel() const;

    void bind(const size_t index = 0) const;
    void unbind() const;

    [[nodiscard]] const texture_config &getTextureConfig() const noexcept;
    [[nodiscard]] const core::EnginePath &getTexturePath() const noexcept;
    [[nodiscard]] bool forRasterizer() const noexcept;

private:

    void parseV2(rcbe::binary::BinaryBuffer &&bb);
    void parseV1(rcbe::binary::BinaryBuffer &&bb);

    texture_config config_;
    core::EnginePath file_path_;

    ColorMapSpecification color_map_spec_;
    ImageSpecification image_spec_;
    ImageBodyType image_body_;

    std::unique_ptr<TextureRasterizerBackendImpl> rasterizer_backend_ = nullptr;
};

using TGATexture = TextureImplementation;
}

#endif //RCBE_ENGINE_TGATEXTURE_HPP
