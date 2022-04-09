#include <optional>

#include <rcbe-engine/datamodel/rendering/buffer_object_helpers.hpp>

namespace rdmn::render {
#ifdef RDMN_VULKAN
uint32_t findMemoryType(VkPhysicalDevice device, uint32_t type_filter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties mem_props;
    vkGetPhysicalDeviceMemoryProperties(device, std::addressof(mem_props));

    for (uint32_t i = 0; i < mem_props.memoryTypeCount; ++i) {
        if (type_filter & (1 << i) &&
            (mem_props.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Can't find suitable memory type!");
}

std::optional<VkCommandBuffer> beginSingleTimeCommands(VkDevice logical_device, VkCommandPool cmd_pool) {
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = cmd_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    if (vkAllocateCommandBuffers(logical_device, std::addressof(alloc_info), std::addressof(command_buffer)) != VK_SUCCESS) {
        return std::nullopt;
    }

    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(command_buffer, std::addressof(begin_info)) != VK_SUCCESS) {
        return std::nullopt;
    }

    return command_buffer;
}

bool endSingleTimeCommands(
        VkDevice logical_device,
        VkCommandBuffer cmd_buff,
        VkQueue graph_queue,
        VkCommandPool cmd_pool
) {
    if (vkEndCommandBuffer(cmd_buff) != VK_SUCCESS) {
        return false;
    }

    VkSubmitInfo sub_info{};
    sub_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    sub_info.commandBufferCount = 1;
    sub_info.pCommandBuffers = std::addressof(cmd_buff);

    if (vkQueueSubmit(graph_queue, 1, std::addressof(sub_info), VK_NULL_HANDLE) != VK_SUCCESS) {
        return false;
    }
    if (vkQueueWaitIdle(graph_queue) != VK_SUCCESS) {
        return false;
    }

    vkFreeCommandBuffers(logical_device, cmd_pool, 1, std::addressof(cmd_buff));
    return true;
}

bool copyBuffer(
        VkDevice logical_device,
        VkBuffer src,
        VkBuffer dst,
        VkDeviceSize size,
        VkCommandPool command_pool,
        VkQueue target_queue
) {
    auto cmd_buff = beginSingleTimeCommands(logical_device, command_pool);
    if (!cmd_buff) {
        BOOST_LOG_TRIVIAL(error) << "Can't begin command buffer!";
        return false;
    }

    auto cb = *cmd_buff;

    VkBufferCopy copy_region{};
    copy_region.dstOffset = 0;
    copy_region.srcOffset = 0;
    copy_region.size = size;
    vkCmdCopyBuffer(cb, src, dst, 1, std::addressof(copy_region));

    const auto res = endSingleTimeCommands(logical_device, cb, target_queue, command_pool);
    if (!res) {
        BOOST_LOG_TRIVIAL(error) << "Can't end command buffer!";
        return false;
    }

    return true;
}

bool createBufferImpl(VkDevice &logical_device,
                      VkPhysicalDevice &device,
                      VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer &buff,
                      VkDeviceMemory &memory) {
    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    auto res = vkCreateBuffer(logical_device, std::addressof(buffer_info), nullptr, std::addressof(buff));
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(error) << "Can't create vertex buffer";
        return false;
    }

    VkMemoryRequirements memory_reqs;
    vkGetBufferMemoryRequirements(logical_device, buff, std::addressof(memory_reqs));

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = memory_reqs.size;
    alloc_info.memoryTypeIndex = findMemoryType(
            device,
            memory_reqs.memoryTypeBits,
            properties);

    res = vkAllocateMemory(logical_device, std::addressof(alloc_info), nullptr, std::addressof(memory));
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(error) << "Can't allocate memory for vertex buffer!";
        return false;
    }

    vkBindBufferMemory(logical_device, buff, memory, 0);

    return true;
}
#elif defined(RDMN_OPENGL)
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
#else

static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);

#endif
}
