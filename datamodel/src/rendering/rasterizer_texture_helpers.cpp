#include <boost/log/trivial.hpp>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/rendering/buffer_object_helpers.hpp>
#include <rcbe-engine/datamodel/rendering/rasterizer_texture_helpers.hpp>

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
    VkDeviceMemory &device_memory) {
    VkImageCreateInfo image_info {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = static_cast<uint32_t>(dims.width);
    image_info.extent.height = static_cast<uint32_t>(dims.height);
    image_info.extent.depth = 1;
    image_info.mipLevels = mip_levels;
    image_info.arrayLayers = 1;
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.samples = num_samples;
    image_info.flags = 0;

    if (vkCreateImage(
            logical_device, std::addressof(image_info), nullptr, std::addressof(image)) != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(error) << "Can't create texture image!";
        return false;
    }

    VkMemoryRequirements mem_req;
    vkGetImageMemoryRequirements(logical_device, image, std::addressof(mem_req));
    VkMemoryAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_req.size;
    alloc_info.memoryTypeIndex = findMemoryType(phys_device, mem_req.memoryTypeBits, properties);

    if (vkAllocateMemory(logical_device, std::addressof(alloc_info), nullptr, std::addressof(device_memory)) != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(error) << "Can't allocate memory for texture image!";
        return false;
    }

    vkBindImageMemory(logical_device, image, device_memory, 0);

    return true;
}

bool transitionImageLayout(
    VkDevice logical_device,
    VkCommandPool cmd_pool,
    VkQueue target_queue,
    VkImage image,
    VkFormat format,
    uint32_t mip_levels,
    VkImageLayout old_layout,
    VkImageLayout new_layout) {
    auto opt_cmd_buff = beginSingleTimeCommands(logical_device, cmd_pool);
    if (!opt_cmd_buff)
        return false;

    auto cmd_buff = *opt_cmd_buff;

    VkImageMemoryBarrier barrier {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mip_levels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = 0;

    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags destination_stage;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        return false;
    }

    vkCmdPipelineBarrier(
        cmd_buff,
        source_stage, destination_stage,
        0, 0,
        nullptr, 0,
        nullptr, 1,
        std::addressof(barrier));

    const auto res = endSingleTimeCommands(logical_device, cmd_buff, target_queue, cmd_pool);
    if (!res)
        return false;

    return true;
}

bool copyBufferToImage(
    VkDevice logical_device,
    VkCommandPool cmd_pool,
    VkQueue target_queue,
    VkBuffer buffer,
    VkImage image,
    rcbe::core::IntegralDimensions dims) {
    auto opt_cmd_buff = beginSingleTimeCommands(logical_device, cmd_pool);
    if (!opt_cmd_buff)
        return false;

    auto cmd_buff = *opt_cmd_buff;

    VkBufferImageCopy region {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        static_cast<uint32_t>(dims.width),
        static_cast<uint32_t>(dims.height),
        1};

    vkCmdCopyBufferToImage(cmd_buff, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, std::addressof(region));

    const auto res = endSingleTimeCommands(logical_device, cmd_buff, target_queue, cmd_pool);
    if (!res)
        return false;

    return true;
}

std::optional<VkImageView> createImageView(
    VkDevice logical_device,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspect_flags,
    uint32_t mip_levels) {
    VkImageViewCreateInfo view_info {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.subresourceRange.aspectMask = aspect_flags;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = mip_levels;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    VkImageView image_view;
    if (vkCreateImageView(logical_device, std::addressof(view_info), nullptr, std::addressof(image_view)) != VK_SUCCESS) {
        return std::nullopt;
    }

    return image_view;
}

bool createTextureImage(
    const rcbe::visual::Texture &texture,
    uint32_t mip_levels,
    VkDevice logical_device,
    VkPhysicalDevice physical_device,
    VkCommandPool command_pool,
    VkQueue graphics_queue,
    VkImage &texture_image,
    VkDeviceMemory &texture_image_memory) {
    VkDeviceSize image_size = texture.getImageSizeBytes();
    const auto &pixels = texture.getPixels();
    rcbe::core::IntegralDimensions dims = {static_cast<int>(texture.getWidth()), static_cast<int>(texture.getHeight())};

    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;
    const auto buffer_created = createBufferImpl(
        logical_device, physical_device, image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        staging_buffer, staging_buffer_memory);

    if (!buffer_created) {
        BOOST_LOG_TRIVIAL(error) << "Can't create staging buffer for texture!";
        return false;
    }

    void *data;
    vkMapMemory(logical_device, staging_buffer_memory, 0, image_size, 0, std::addressof(data));
    std::memcpy(data, reinterpret_cast<const uint8_t *>(pixels.rawData()), static_cast<size_t>(image_size));
    vkUnmapMemory(logical_device, staging_buffer_memory);

    const auto image_created = createImage(
        logical_device,
        physical_device,
        dims,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        mip_levels,
        VK_SAMPLE_COUNT_1_BIT,
        texture_image,
        texture_image_memory);

    if (!image_created) {
        BOOST_LOG_TRIVIAL(error) << "Image wasn't created!";
        return false;
    }

    const auto layout_transitioned = transitionImageLayout(
        logical_device,
        command_pool,
        graphics_queue,
        texture_image,
        VK_FORMAT_R8G8B8A8_SRGB,
        mip_levels,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    if (!layout_transitioned) {
        BOOST_LOG_TRIVIAL(error) << "Can't transition image layout!";
        return false;
    }

    const auto buff_copied = copyBufferToImage(
        logical_device,
        command_pool,
        graphics_queue,
        staging_buffer,
        texture_image,
        dims);

    if (!buff_copied) {
        BOOST_LOG_TRIVIAL(error) << "Can't copy buffer to image!";
        return false;
    }

    const auto mipmaps_generated = generateMipmaps(
        logical_device,
        command_pool,
        graphics_queue,
        texture_image,
        texture.getWidth(),
        texture.getHeight(),
        mip_levels);

    if (!mipmaps_generated) {
        BOOST_LOG_TRIVIAL(error) << "Can't generate mip maps!";
        return false;
    }

    vkDestroyBuffer(logical_device, staging_buffer, nullptr);
    vkFreeMemory(logical_device, staging_buffer_memory, nullptr);

    return true;
}

bool createTextureSampler(
    VkDevice logical_device,
    VkSampler &texture_sampler,
    uint32_t mip_levels) {
    VkSamplerCreateInfo sampler_info {};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.anisotropyEnable = VK_TRUE;
    sampler_info.maxAnisotropy = 16;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable = VK_FALSE;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias = 0.0f;
    sampler_info.minLod = 0.0f;/// Change to half of mip levels to see effect
    sampler_info.maxLod = static_cast<float>(mip_levels);

    if (vkCreateSampler(logical_device, std::addressof(sampler_info), nullptr, std::addressof(texture_sampler)) != VK_SUCCESS) {
        return false;
    }

    return true;
}

bool generateMipmaps(
    VkDevice logical_device,
    VkCommandPool command_pool,
    VkQueue graph_queue,
    VkImage image,
    uint32_t width,
    uint32_t height,
    uint32_t mip_levels) {
    auto command_buff_opt = rdmn::render::beginSingleTimeCommands(logical_device, command_pool);
    if (!command_buff_opt)
        return false;

    const auto cmd_buff = *command_buff_opt;
    VkImageMemoryBarrier barrier {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    auto mip_width = static_cast<int32_t>(width);
    auto mip_height = static_cast<int32_t>(height);

    for (size_t i = 1; i < mip_levels; ++i) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(
            cmd_buff,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, std::addressof(barrier));

        VkImageBlit blit {};
        blit.srcOffsets[0] = {0, 0, 0};
        blit.srcOffsets[1] = {mip_width, mip_height, 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;

        blit.dstOffsets[0] = {0, 0, 0};
        blit.dstOffsets[1] = {
            mip_width > 1 ? mip_width / 2 : 1,
            mip_height > 1 ? mip_height / 2 : 1,
            1};
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(
            cmd_buff,
            image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, std::addressof(blit),
            VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(
            cmd_buff,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, std::addressof(barrier));

        if (mip_width > 1) mip_width /= 2;
        if (mip_height > 1) mip_height /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mip_levels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(
        cmd_buff,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, std::addressof(barrier));

    return endSingleTimeCommands(logical_device, cmd_buff, graph_queue, command_pool);
}

uint32_t calculateMipLevels(size_t tex_width, size_t tex_height) {
    return (static_cast<uint32_t>(std::floor(std::log2(std::max(tex_width, tex_height)))) + 1);
}
}// namespace rdmn::render