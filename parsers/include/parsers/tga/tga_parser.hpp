#ifndef RCBE_ENGINE_TGA_PARSER_HPP
#define RCBE_ENGINE_TGA_PARSER_HPP

#include <rcbe-engine/datamodel/visual/image_data.hpp>
#include <rcbe-engine/fundamentals/types.hpp>

namespace rdmn::parse::tga {
struct extended_imagedata {
    struct colormap_specification {
        static constexpr size_t SIZE = sizeof(uint16_t) * 2 + sizeof(uint8_t);

        uint16_t first_entry_index;
        uint16_t color_map_length;
        uint8_t color_map_entry_size;
    } colormap_spec;
    vis::image_data general_metadata;
};

vis::image_data parse(const rcbe::core::EnginePath &file_path);
extended_imagedata extended_parse(const rcbe::core::EnginePath &file_path);
}// namespace rdmn::parse::tga

#endif//RCBE_ENGINE_TGA_PARSER_HPP
