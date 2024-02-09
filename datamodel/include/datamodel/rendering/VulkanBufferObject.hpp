#ifndef RCBE_ENGINE_VULKANBUFFEROBJECT_HPP
#define RCBE_ENGINE_VULKANBUFFEROBJECT_HPP

#include <array>

#include <vulkan/vulkan.hpp>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/geometry/Mesh.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#ifdef RDMN_GLM_INTEGRATION
#include <glm/ext/matrix_float4x4.hpp>
#endif

namespace rdmn::render {
namespace {
inline constexpr size_t UNIFORM_BUFFER_MEMBER_DEFAULT_ALIGNMENT = 16;
}

class VulkanVertexBufferObject {
public:

    using ValueType = float;
    using IndexType = uint32_t;
    using StorageType = std::vector<ValueType>;
    using IndicesStorageType = std::vector<IndexType>;
    using RawDataType = const ValueType *;
    using BindingDescriptionType = VkVertexInputBindingDescription;
    using BindingAttributeType = std::array<VkVertexInputAttributeDescription, 4>;

    VulkanVertexBufferObject() = delete;
    VulkanVertexBufferObject(
        const std::vector<rcbe::geometry::Mesh> &meshes,
        VkDevice &logical_device,
        VkPhysicalDevice &device,
        VkQueue &presentation_queue,
        VkCommandPool &command_pool);
    ~VulkanVertexBufferObject();

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] size_t indicesSize() const noexcept;
    [[nodiscard]] size_t sourceSize() const noexcept;
    [[nodiscard]] size_t sizeBytes() const noexcept;

    [[nodiscard]] size_t vertsByteSize() const noexcept;
    [[nodiscard]] size_t normsByteSize() const noexcept;
    [[nodiscard]] size_t colorsByteSize() const noexcept;
    [[nodiscard]] size_t texCoordByteSize() const noexcept;

    void bind(VkCommandBuffer command_buffer, const size_t voffset, const size_t ioffset);

    void deleteBuffer();

    static BindingDescriptionType getBindingDescription();
    static BindingAttributeType getAttributesDescription();
    static size_t getStride();

private:

    bool buffer_set_ = false;

    size_t source_size_;// since size of all filled containers (i.e. vertices, normals, colors) should be the same we keep only one size
    size_t buffer_size_bytes_;

    size_t vertices_byte_size_;
    size_t normals_byte_size_;
    size_t colors_byte_size_;
    size_t tex_coords_byte_size_;

    size_t vertices_size_;
    size_t normals_size_;
    size_t colors_size_;
    size_t texcoord_size_;

    StorageType buffer_data_;
    IndicesStorageType indices_;

    VkBuffer vertex_buffer_;
    VkBuffer index_buffer_;
    VkDeviceMemory vertex_buffer_memory_;
    VkDeviceMemory index_buffer_memory_;

    VkDevice logical_device_;
};

class VulkanUniformBufferObject {
public:

    VulkanUniformBufferObject() = delete;
    ~VulkanUniformBufferObject();

    VulkanUniformBufferObject(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkDescriptorSetLayout desc_set_layout,
        VkImageView image_view,
        VkSampler sampler,
        const size_t image_amount);

    VulkanUniformBufferObject(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkDescriptorSetLayout desc_set_layout,
        std::vector<VkImageView> &&image_view,
        std::vector<VkSampler> &&sampler,
        const size_t image_amount);

    VulkanUniformBufferObject(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkDescriptorSetLayout desc_set_layout,
        VkSampler global_sampler,
        std::vector<VkImageView> &&image_view,
        const size_t total_textures,
        const size_t image_amount);

    void deleteBuffer();
    void update(
        const rcbe::math::Transform &view,
        const rcbe::math::Transform &model,
        const rcbe::math::Matrix4x4f &perspective,
        const uint32_t image_index);

    void bind(VkCommandBuffer command_buffer, VkPipelineLayout pipeline_layout, const size_t image_index);

    [[nodiscard]] VkBuffer getBufferAt(const size_t index) const;

    [[nodiscard]] size_t sizeofBuffObject() const;

    [[nodiscard]] VkDescriptorSet getDescriptorSet(const size_t index) const;

private:

    struct uniform_buffer_object {
        // these have to be column major, due to all APIs using col major
        // my matrices are row major, so this ones are actually transposed
        alignas(UNIFORM_BUFFER_MEMBER_DEFAULT_ALIGNMENT) rcbe::math::Matrix4x4f model;
        alignas(UNIFORM_BUFFER_MEMBER_DEFAULT_ALIGNMENT) rcbe::math::Matrix4x4f view;
        alignas(UNIFORM_BUFFER_MEMBER_DEFAULT_ALIGNMENT) rcbe::math::Matrix4x4f proj;
    };

    bool createDescriptorPool(VkDevice logical_device, const size_t image_amount, const ssize_t total_textures = -1);
    bool createDescriptorSets(
        VkDevice logical_device,
        VkDescriptorSetLayout desc_set_layout,
        const size_t image_amount);

    bool createDescriptorSets(
        VkDevice logical_device,
        VkDescriptorSetLayout desc_set_layout,
        VkSampler global_sampler,
        const size_t image_amount,
        const size_t total_textures);

    VkDevice logical_device_;

    std::vector<VkBuffer> uniform_buffers_;
    std::vector<VkDeviceMemory> uniform_buffers_memory_;

    VkDescriptorPool descriptor_pool_;
    std::vector<VkDescriptorSet> descriptor_sets_;
    std::vector<VkImageView> image_view_;
    std::vector<VkSampler> sampler_;
    VkSampler global_sampler_;

    bool buffer_created_ = false;
};

}// namespace rdmn::render

#endif//RCBE_ENGINE_VULKANBUFFEROBJECT_HPP
