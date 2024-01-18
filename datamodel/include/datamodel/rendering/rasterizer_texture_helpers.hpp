#ifndef RCBE_ENGINE_RASTERIZER_TEXTURE_HELPERS_HPP
#define RCBE_ENGINE_RASTERIZER_TEXTURE_HELPERS_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>

namespace rdmn::render {
bool createImage(
    VkDevice logical_device,
    VkPhysicalDevice phys_device,
    rcbe::core::IntegralDimensions dims,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    uint32_t mip_levels,
    VkSampleCountFlagBits num_samples,
    VkImage &image,
    VkDeviceMemory &device_memory);
bool transitionImageLayout(
    VkDevice logical_device,
    VkCommandPool cmd_pool,
    VkQueue target_queue,
    VkImage image,
    VkFormat format,
    uint32_t mip_levels,
    VkImageLayout old_layout,
    VkImageLayout new_layout);
bool copyBufferToImage(
    VkDevice logical_device,
    VkCommandPool cmd_pool,
    VkQueue target_queue,
    VkBuffer buffer,
    VkImage image,
    rcbe::core::IntegralDimensions dims);

std::optional<VkImageView> createImageView(
    VkDevice logical_device,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspect_flags,
    uint32_t mip_levels);

bool createTextureImage(
    const rcbe::visual::Texture &texture,
    uint32_t mip_levels,
    VkDevice logical_device,
    VkPhysicalDevice physical_device,
    VkCommandPool command_pool,
    VkQueue graphics_queue,
    VkImage &texture_image,
    VkDeviceMemory &texture_image_memory);

bool createTextureSampler(
    VkDevice logical_device,
    VkSampler &texture_sampler,
    uint32_t mip_levels);

bool generateMipmaps(
    VkDevice logical_device,
    VkCommandPool command_pool,
    VkQueue graph_queue,
    VkImage image,
    uint32_t width,
    uint32_t height,
    uint32_t mip_levels);

uint32_t calculateMipLevels(size_t tex_width, size_t tex_height);
}// namespace rdmn::render

#endif//RCBE_ENGINE_RASTERIZER_TEXTURE_HELPERS_HPP
