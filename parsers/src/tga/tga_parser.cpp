#include <rdmn-engine/logger/trivial_logger.hpp>

#include <rcbe-engine/binary/BinaryBuffer.hpp>
#include <rcbe-engine/exceptions/not_implemented.hpp>
#include <rcbe-engine/parsers/tga/tga_parser.hpp>

namespace {
namespace tga = rdmn::parse::tga;

inline constexpr const char *TGA_VERSION_TWO_TOKEN = "TRUEVISION-XFILE";

struct image_specification {
    static constexpr size_t SIZE = sizeof(uint16_t) * 4 + sizeof(uint8_t) * 2;

    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t pixel_depth;
    uint8_t image_descriptor;
};


tga::extended_imagedata parseV2(rcbe::binary::BinaryBuffer &&bb) {
    throw rcbe::exception::NotImplementedException("Parsing TGA V2 is not implemented!");
}

tga::extended_imagedata parseV1(rcbe::binary::BinaryBuffer &&bb) {
    size_t offset = 0;
    const auto id_length = bb.at(offset, sizeof(uint8_t)).get<uint8_t>();

    const auto color_map_type = bb.at(offset, sizeof(uint8_t)).get<uint8_t>();

    const auto image_type = bb.at(offset, sizeof(uint8_t)).get<uint8_t>();

    const auto color_map_spec = bb.at(
                                      offset,
                                      tga::extended_imagedata::colormap_specification::SIZE)
                                    .get<tga::extended_imagedata::colormap_specification>();

    const auto image_spec = bb.at(offset, image_specification::SIZE).get<image_specification>();

    using ImageBodyType = decltype(rdmn::vis::image_data::pixels)::element_type;
    auto image_body = ImageBodyType {image_spec.height, image_spec.width};

    if (color_map_type == 0) {
        const auto total_pixels = image_spec.width * image_spec.height;
        const auto pixel_depth_bytes = image_spec.pixel_depth / 8;
        
        for (size_t i = 0; i < total_pixels; ++i) {
            std::vector<uint8_t> color_comp_bytes;
            color_comp_bytes.reserve(pixel_depth_bytes + 1);
            color_comp_bytes.resize(pixel_depth_bytes, 0);

            for (size_t j = 0; j < pixel_depth_bytes; ++j) {
                color_comp_bytes[j] = bb.at(offset, sizeof(uint8_t)).get<uint8_t>();
            }

            bool alpha_ch_missing = pixel_depth_bytes < sizeof(ImageBodyType ::ValueType);
            /// TODO: implement file endianess check, or clarify that TGA is always in big endian @sckorn
            /// Below is the fix for endianess (for lil' endian)
            if (alpha_ch_missing)
                std::reverse(color_comp_bytes.begin(), color_comp_bytes.end());
            else {
                std::swap(color_comp_bytes[0], color_comp_bytes[color_comp_bytes.size() - 2]);
            }

            // If one one more component is missing, i.e. pixel_depth_bytes is < 4,
            // we need to add at least one more byte to the end.
            if (alpha_ch_missing)
                color_comp_bytes.push_back(std::numeric_limits<uint8_t>::max());

            ImageBodyType ::ValueType val = *reinterpret_cast<ImageBodyType::ValueType *>(color_comp_bytes.data());

            image_body.pushBack(val);
        }

        // TGA is RGB(A) by default
        const auto comp_order = rcbe::visual::TextureComponentOrder::RGBA;

        return {
            .colormap_spec {
                .first_entry_index = color_map_spec.first_entry_index,
                .color_map_length = color_map_spec.color_map_length,
                .color_map_entry_size = color_map_spec.color_map_entry_size,
            },
            .general_metadata {
                .metadata = {
                    .origin = {image_spec.x_origin, image_spec.y_origin},
                    .dimensions = {.width = image_spec.width, .height = image_spec.height},
                    .pixel_depth_bytes = static_cast<uint8_t>(pixel_depth_bytes),
                    .component_order = comp_order,
                },
                .pixels = rdmn::vis::image_data::wrapPixelsInStorage(std::move(image_body))},
        };
    } else {
        throw std::runtime_error("Parsing TGA V1 with colormap is not implemented!");
    }
}
}// namespace


namespace rcbe::binary {
template <>
void to_binary(rcbe::binary::BinaryBuffer &bb, const tga::extended_imagedata::colormap_specification &v) {
    bb = {
        v.first_entry_index,
        v.color_map_length,
        v.color_map_entry_size};
}

template <>
void from_binary(const rcbe::binary::BinaryBuffer &bb, tga::extended_imagedata::colormap_specification &v) {
    v.first_entry_index = bb.at(0, sizeof(uint16_t)).get<uint16_t>();
    v.color_map_length = bb.at(sizeof(uint16_t), sizeof(uint16_t)).get<uint16_t>();
    v.color_map_entry_size = bb.at(sizeof(uint16_t) * 2, sizeof(uint8_t)).get<uint8_t>();
}

template <>
void to_binary(rcbe::binary::BinaryBuffer &bb, const image_specification &v) {
    bb = {
        v.x_origin,
        v.y_origin,
        v.width,
        v.height,
        v.pixel_depth,
        v.image_descriptor};
}

template <>
void from_binary(const rcbe::binary::BinaryBuffer &bb, image_specification &v) {
    v.x_origin = bb.at(0, sizeof(uint16_t)).get<uint16_t>();
    v.y_origin = bb.at(sizeof(uint16_t), sizeof(uint16_t)).get<uint16_t>();
    v.width = bb.at(sizeof(uint16_t) * 2, sizeof(uint16_t)).get<uint16_t>();
    v.height = bb.at(sizeof(uint16_t) * 3, sizeof(uint16_t)).get<uint16_t>();
    v.pixel_depth = bb.at(sizeof(uint16_t) * 4, sizeof(uint8_t)).get<uint8_t>();
    v.image_descriptor = bb.at(sizeof(uint16_t) * 4 + sizeof(uint8_t), sizeof(uint8_t)).get<uint8_t>();
}
}// namespace rcbe::binary

namespace rdmn::parse::tga {

vis::image_data parse(const rcbe::core::EnginePath &file_path) {
    const auto [ext_data, gen_data] = extended_parse(file_path);
    return {
        .metadata {
            .origin = gen_data.metadata.origin,
            .dimensions = gen_data.metadata.dimensions,
            .pixel_depth_bytes = gen_data.metadata.pixel_depth_bytes,
            .component_order = gen_data.metadata.component_order,
        },
        .pixels = rdmn::vis::image_data::wrapPixelsInStorage(gen_data.pixels->clone()),
    };
}

extended_imagedata extended_parse(const rcbe::core::EnginePath &file_path) {
    if (!std::filesystem::exists(file_path))
        throw std::runtime_error("File " + file_path.string() + " doesn't exist!");

    if (!std::filesystem::is_regular_file(file_path))
        throw std::runtime_error(file_path.string() + " is not a path!");

    if (file_path.extension().string() != ".tga" && file_path.extension().string() != ".TGA")
        throw std::runtime_error("Only supports TGA format!");

    std::ifstream ifs {file_path, std::ios_base::binary};

    if (!ifs) 
        throw std::runtime_error("Can't open " + file_path.string() + " for reading!");

    rcbe::binary::BinaryBuffer bb;
    ifs >> bb;

    auto check_str = bb.at(bb.size() - 19, 16).get<std::string>();

    RDMN_LOG(RDMN_LOG_DEBUG) << "Check str is " << check_str << " check string size " << check_str.size();

    if (check_str == std::string(TGA_VERSION_TWO_TOKEN)) {
        RDMN_LOG(RDMN_LOG_DEBUG) << "Version 2 TGA file detected!";
        return parseV2(std::move(bb));
    } else {
        RDMN_LOG(RDMN_LOG_DEBUG) << "Version 1 TGA file detected!";
        return parseV1(std::move(bb));
    }
}
}// namespace rdmn::parse::tga