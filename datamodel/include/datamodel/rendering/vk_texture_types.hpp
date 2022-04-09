#ifndef RCBE_ENGINE_VK_TEXTURE_TYPES_HPP
#define RCBE_ENGINE_VK_TEXTURE_TYPES_HPP

#include <vulkan/vulkan.hpp>

#include <rcbe-engine/datamodel/visual/texture_types.hpp>

namespace rdmn::render {
enum class TextureWrappingType {
    repeat = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    mirrored_repeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    clamp_to_edge = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    clamp_to_border = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    mirrored_clamp_to_edge = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
};

enum class TextureType {
    texture_1d = VK_IMAGE_TYPE_1D,
    texture_2d = VK_IMAGE_TYPE_2D,
    texture_3d = VK_IMAGE_TYPE_3D
};

enum class TextureFilteringType {
    nearest = VK_FILTER_NEAREST,
    linear = VK_FILTER_LINEAR,
    cubic = VK_FILTER_CUBIC_IMG,
};

struct rasterizer_texture_config {
    TextureWrappingType wrapping_type = TextureWrappingType::repeat;
    TextureType texture_type = TextureType::texture_2d;
    TextureFilteringType filtering_type = TextureFilteringType::linear;
    rcbe::visual::texture_config image_config;
};
}

#endif //RCBE_ENGINE_VK_TEXTURE_TYPES_HPP
