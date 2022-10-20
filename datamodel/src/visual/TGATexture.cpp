#include "TGATexture.hpp"

#include <filesystem>
#include <string>

#include <boost/log/trivial.hpp>

#include <rcbe-engine/binary/BinaryBuffer.hpp>
#include <rcbe-engine/exceptions/not_implemented.hpp>

static constexpr const char * TGA_VERSION_TWO_TOKEN = "TRUEVISION-XFILE";

namespace rcbe::binary {
template <>
void to_binary(rcbe::binary::BinaryBuffer &bb, const rcbe::visual::TextureImplementation::ColorMapSpecification &v) {
    bb = {
      v.first_entry_index,
      v.color_map_length,
      v.color_map_entry_size
    };
}

template <>
void from_binary(const rcbe::binary::BinaryBuffer &bb, rcbe::visual::TextureImplementation::ColorMapSpecification &v) {
    v.first_entry_index = bb.at(0, sizeof(uint16_t)).get<uint16_t>();
    v.color_map_length = bb.at(sizeof(uint16_t), sizeof(uint16_t)).get<uint16_t>();
    v.color_map_entry_size = bb.at(sizeof(uint16_t) * 2, sizeof(uint8_t)).get<uint8_t>();
}

template <>
void to_binary(rcbe::binary::BinaryBuffer &bb, const rcbe::visual::TextureImplementation::ImageSpecification &v) {
    bb = {
            v.x_origin,
            v.y_origin,
            v.width,
            v.height,
            v.pixel_depth,
            v.image_descriptor
    };
}

template <>
void from_binary(const rcbe::binary::BinaryBuffer &bb, rcbe::visual::TextureImplementation::ImageSpecification &v) {
    v.x_origin = bb.at(0, sizeof(uint16_t)).get<uint16_t>();
    v.y_origin = bb.at(sizeof(uint16_t), sizeof(uint16_t)).get<uint16_t>();
    v.width = bb.at(sizeof(uint16_t) * 2, sizeof(uint16_t)).get<uint16_t>();
    v.height = bb.at(sizeof(uint16_t) * 3, sizeof(uint16_t)).get<uint16_t>();
    v.pixel_depth = bb.at(sizeof(uint16_t) * 4, sizeof(uint8_t)).get<uint8_t>();
    v.image_descriptor = bb.at(sizeof(uint16_t) * 4 + sizeof(uint8_t), sizeof(uint8_t)).get<uint8_t>();
}
}

namespace rcbe::visual {
TextureImplementation::TextureImplementation(
        const core::EnginePath &path,
        texture_config config,
        bool for_rasterizer
)
:
config_ (config)
, file_path_(path)
{
    if (!std::filesystem::exists(file_path_))
        throw std::runtime_error("File " + file_path_.string() + " doesn't exist!");

    if (!std::filesystem::is_regular_file(file_path_))
        throw std::runtime_error(file_path_.string() + " is not a path!");

    if (file_path_.extension().string() != ".tga" && file_path_.extension().string() != ".TGA")
        throw std::runtime_error("Only supports TGA format!");

    std::ifstream ifs { file_path_ };

    if (!ifs)
        throw std::runtime_error("Can't open " + file_path_.string() + " for reading!");

    binary::BinaryBuffer bb;
    ifs >> bb;

    auto check_str = bb.at(bb.size() - 19, 16).get<std::string>();

    BOOST_LOG_TRIVIAL(debug) << "Check str is " << check_str << " check string size " << check_str.size();

    if (check_str == std::string(TGA_VERSION_TWO_TOKEN)) {
        BOOST_LOG_TRIVIAL(debug) << "Version 2 TGA file detected!";
        parseV2(std::move(bb));
    } else {
        BOOST_LOG_TRIVIAL(debug) << "Version 1 TGA file detected!";
        parseV1(std::move(bb));
    }

    if (for_rasterizer)
        rasterizer_backend_ = std::make_unique<TextureRasterizerBackendImpl>(
                reinterpret_cast<const uint8_t*>(getBody().rawData()),
                config_,
                getWidth(),
                getHeight()
                );
}

void TextureImplementation::parseV2(rcbe::binary::BinaryBuffer &&bb) {
    throw rcbe::exception::NotImplementedException("Method TGATexture::parseV2 is not implemented!");
}

void TextureImplementation::parseV1(rcbe::binary::BinaryBuffer &&bb) {
    size_t offset = 0;
    const auto id_length = bb.at(offset, sizeof(uint8_t)).get<uint8_t>();

    const auto color_map_type = bb.at(offset, sizeof(uint8_t)).get<uint8_t>();

    const auto image_type = bb.at(offset, sizeof(uint8_t)).get<uint8_t>();

    color_map_spec_ = bb.at(offset, ColorMapSpecification::SIZE).get<ColorMapSpecification>();

    image_spec_ = bb.at(offset, ImageSpecification::SIZE).get<ImageSpecification>();

    image_body_ = ImageBodyType {image_spec_.height, image_spec_.width};

    if (color_map_type == 0) {
        BOOST_LOG_TRIVIAL(warning) << "No color map data available!";

        const auto total_pixels = image_spec_.width * image_spec_.height;
        const auto pixel_depth_bytes = image_spec_.pixel_depth / 8;

        for (size_t i = 0; i < total_pixels; ++i) {
            std::vector<uint8_t> color_comp_bytes(pixel_depth_bytes, 0);

            for (size_t j = 0; j < pixel_depth_bytes; ++j) {
                color_comp_bytes[j] = bb.at(offset, sizeof(uint8_t)).get<uint8_t>();
            }

            // TGA is RGB(A) by default, so if we need GBR(A), we need to reverse the order of bytes
            if (config_.component_order != decltype(config_.component_order)::GBR &&
                    config_.component_order != decltype(config_.component_order)::GBRA)
                std::reverse(color_comp_bytes.begin(), color_comp_bytes.end());

            // If one one more component is missing, i.e. pixel_depth_bytes is < 4,
            // we need to add at least one more byte to the end.
            if (pixel_depth_bytes < sizeof(ImageBodyType ::ValueType))
                color_comp_bytes.push_back(std::numeric_limits<uint8_t>::max());

            ImageBodyType ::ValueType val = *reinterpret_cast<ImageBodyType::ValueType*>(color_comp_bytes.data());

            image_body_.pushBack(val);
        }
    }
}

const TextureImplementation::ImageBodyType &TextureImplementation::getBody() const {
    return image_body_;
}

const TextureImplementation::ColorMapSpecification &TextureImplementation::getColorMapSpec() const {
    return color_map_spec_;
}

const TextureImplementation::ImageSpecification &TextureImplementation::getImageSpec() const {
    return image_spec_;
}

size_t TextureImplementation::getWidth() const {
    return getImageSpec().width;
}

size_t TextureImplementation::getHeight() const {
    return getImageSpec().height;
}

size_t TextureImplementation::getBitsPerPixel() const {
    return getImageSpec().pixel_depth;
}

void TextureImplementation::bind(const size_t index) const {
    if (!rasterizer_backend_)
        throw std::runtime_error("Can't bind texture not configured for rasterizer");

    rasterizer_backend_->bind(index);
}

void TextureImplementation::unbind() const {
    if (!rasterizer_backend_)
        throw std::runtime_error("Can't unbind texture not configured for rasterizer");

    rasterizer_backend_->unbind();
}

const texture_config &TextureImplementation::getTextureConfig() const noexcept {
    return config_;
}

const core::EnginePath &TextureImplementation::getTexturePath() const noexcept {
    return file_path_;
}

bool TextureImplementation::forRasterizer() const noexcept {
    return static_cast<bool>(rasterizer_backend_);
}

}