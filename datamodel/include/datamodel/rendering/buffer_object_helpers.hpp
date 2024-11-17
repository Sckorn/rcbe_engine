#ifndef RCBE_ENGINE_BUFFER_OBJECT_HELPERS_HPP
#define RCBE_ENGINE_BUFFER_OBJECT_HELPERS_HPP

#include <optional>
#include <vector>

#include <rdmn-engine/logger/trivial_logger.hpp>

#include <rdmn-engine/public_api.hpp>

#include <rcbe-engine/datamodel/geometry/Mesh.hpp>
#include <rcbe-engine/fundamentals/convinience.hpp>
#include <rcbe-engine/fundamentals/types.hpp>

#ifdef RDMN_VULKAN
#include <vulkan/vulkan.hpp>
#elif defined(RDMN_OPENGL)
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
#else
static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif

namespace rdmn::render {
#ifdef RDMN_VULKAN
R_PUBLIC_API uint32_t findMemoryType(VkPhysicalDevice device, uint32_t type_filter, VkMemoryPropertyFlags properties);
R_PUBLIC_API std::optional<VkCommandBuffer> beginSingleTimeCommands(VkDevice logical_device, VkCommandPool cmd_pool);
R_PUBLIC_API bool endSingleTimeCommands(
    VkDevice logical_device,
    VkCommandBuffer cmd_buff,
    VkQueue graph_queue,
    VkCommandPool cmd_pool);
R_PUBLIC_API bool copyBuffer(
    VkDevice logical_device,
    VkBuffer src,
    VkBuffer dst,
    VkDeviceSize size,
    VkCommandPool command_pool,
    VkQueue target_queue);
R_PUBLIC_API bool createBufferImpl(
    VkDevice &logical_device,
    VkPhysicalDevice &device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer &buff,
    VkDeviceMemory &memory);

template <typename Container>
bool createGeneralBuffer(size_t buffer_size,
                         VkDevice &logical_device,
                         VkPhysicalDevice &device,
                         VkBuffer &buff,
                         VkDeviceMemory &memory,
                         VkQueue &presentation_queue,
                         VkCommandPool &command_pool,
                         VkBufferUsageFlagBits usage,
                         const Container &source) {
    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;
    auto res = createBufferImpl(
        logical_device,
        device,
        buffer_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        staging_buffer,
        staging_buffer_memory);

    if (!res) {
        RDMN_LOG(RDMN_LOG_ERROR) << "Can't create buffer!";
        return false;
    }

    void *data;
    vkMapMemory(logical_device, staging_buffer_memory, 0, buffer_size, 0, std::addressof(data));
    memcpy(data, source.data(), buffer_size);
    vkUnmapMemory(logical_device, staging_buffer_memory);

    if (!createBufferImpl(
            logical_device,
            device,
            buffer_size,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                usage,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            buff,
            memory)) {
        RDMN_LOG(RDMN_LOG_ERROR) << "Can't create buffer object";
        return false;
    }

    auto buffer_copied = copyBuffer(logical_device, staging_buffer, buff, buffer_size, command_pool, presentation_queue);
    if (!buffer_copied) {
        RDMN_LOG(RDMN_LOG_ERROR) << "Buffer couldn't be copied!";
        return false;
    }

    vkDestroyBuffer(logical_device, staging_buffer, nullptr);
    vkFreeMemory(logical_device, staging_buffer_memory, nullptr);

    return true;
}
#elif defined(RDMN_OPENGL)
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
#else
static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif

template <typename Value, typename Index>
struct BufferObjectData {
    using ValueType = Value;

    std::vector<ValueType> vertices;
    std::vector<ValueType> normals;
    std::vector<ValueType> colors;
    std::vector<ValueType> tex_coords;

    std::vector<Index> facets;

    ssize_t source_size;

    bool normals_intact = false;

    size_t vertices_byte_size;
    size_t normals_byte_size;
    size_t colors_byte_size;
    size_t texcoords_byte_size;

    size_t buffer_byte_size;

    R_PUBLIC_API void printSizes() const {
        RDMN_LOG(RDMN_LOG_DEBUG) << "Vertices size " << vertices.size();
        RDMN_LOG(RDMN_LOG_DEBUG) << "Normals size " << normals.size();
        RDMN_LOG(RDMN_LOG_DEBUG) << "Colors size " << colors.size();
        RDMN_LOG(RDMN_LOG_DEBUG) << "Texture coordinates size " << tex_coords.size();
    }
};

struct R_PUBLIC_API buffer_object_data_extraction_options {
    bool non_transformed = true;
    bool dont_use_facets = true;
};

template <typename ValueType, typename IndexType>
BufferObjectData<ValueType, IndexType> extractBufferObjectData(
    const std::vector<rcbe::geometry::Mesh> &meshes,
    buffer_object_data_extraction_options options = {}) {
    BufferObjectData<ValueType, IndexType> ret {};

    size_t normals_size = 0;
    ret.source_size = std::accumulate(meshes.begin(), meshes.end(), 0, [&normals_size](auto sum, const auto &s) mutable {
        normals_size += s.normalsSize();
        return sum + s.verticesSize();
    });

    ret.normals_intact = (ret.source_size == normals_size);

    ret.vertices.reserve(ret.source_size * 3);
    ret.colors.reserve(ret.source_size * 3);
    ret.tex_coords.reserve(ret.source_size * 2);

    if (!ret.normals_intact) {
        RDMN_LOG(RDMN_LOG_WARN) << "Normals are of wrong size!";
    } else {
        ret.normals.reserve(ret.source_size * 3);
    }

    for (const auto &m : meshes) {
        // The below transformations are needed, because:
        // when we don't use shaders, mesh vertices should be
        // transformed into "world" coordinates right off the bat,
        // in case of shaders, this transformation is done inside the shader
        // using mesh model transform.
        const auto &vertices = (m.verticesTransformed() && options.non_transformed) ? m.verticesOriginal() : m.vertices();
        const auto &normals = (m.verticesTransformed() && options.non_transformed) ? m.normalsOriginal() : m.normals();
        const auto &color = m.color();
        const auto &facets = m.facets();
        const auto &tex_coord = m.texCoord();

        for (const auto &v : vertices) {
            ret.vertices.push_back(v.x());
            ret.vertices.push_back(v.y());
            ret.vertices.push_back(v.z());

            ret.colors.push_back(color.r());
            ret.colors.push_back(color.g());
            ret.colors.push_back(color.b());
            ret.colors.push_back(color.a());
        }

        for (const auto &n : normals) {
            ret.normals.push_back(n.x());
            ret.normals.push_back(n.y());
            ret.normals.push_back(n.z());
        }

        for (const auto &f : facets) {
            ret.facets.push_back(f.indices[0]);
            ret.facets.push_back(f.indices[1]);
            ret.facets.push_back(f.indices[2]);

            if (!options.dont_use_facets) {
                for (size_t i = 0; i < 3; ++i) {
                    if (!tex_coord.empty()) {
                        const auto &tc = tex_coord.at(f.tex_coords_indices[i]);

                        ret.tex_coords.push_back(tc.x());
                        ret.tex_coords.push_back(tc.y());
                    } else {
                        ret.tex_coords.push_back(0);
                        ret.tex_coords.push_back(0);
                    }
                }
            }
        }

        if (options.dont_use_facets) {
            for (const auto &tc : tex_coord) {
                ret.tex_coords.push_back(tc.x());
                ret.tex_coords.push_back(tc.y());
            }
        }
    }

    ret.printSizes();

    ret.vertices_byte_size = (sizeof(typename decltype(ret.vertices)::value_type) * ret.vertices.size());
    ret.normals_byte_size = (sizeof(typename decltype(ret.normals)::value_type) * ret.normals.size());
    ret.colors_byte_size = (sizeof(typename decltype(ret.colors)::value_type) * ret.colors.size());
    ret.texcoords_byte_size = (sizeof(typename decltype(ret.tex_coords)::value_type) * ret.tex_coords.size());

    ret.buffer_byte_size = ret.vertices_byte_size + ret.normals_byte_size + ret.colors_byte_size + ret.texcoords_byte_size;

    return ret;
}

}// namespace rdmn::render

#endif//RCBE_ENGINE_BUFFER_OBJECT_HELPERS_HPP
