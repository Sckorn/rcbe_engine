#ifndef RDMN_VULKANGRAPHICSPIPELINE_HPP
#define RDMN_VULKANGRAPHICSPIPELINE_HPP

#include <memory>

#include <vulkan/vulkan.hpp>

#include <rcbe-engine/datamodel/rendering/Material.hpp>

namespace rdmn::render {

class VulkanVertexBufferObject;

class VulkanGraphicsPipeline {
public:

    VulkanGraphicsPipeline() = delete;
    ~VulkanGraphicsPipeline();

    VulkanGraphicsPipeline(
        const rcbe::rendering::Material &material,
        VkDevice logical_device,
        VkExtent2D extent,
        VkSampleCountFlagBits msaa_samples,
        VkDescriptorSetLayout desc_set_layout,
        VkRenderPass render_pass,
        size_t push_const_sizeof);

    [[nodiscard]] const VkPipeline &getPipeline() const noexcept;
    [[nodiscard]] const VkPipelineLayout &getLayout() const noexcept;

private:

    VkDevice logical_device_;
    VkPipeline pipeline_;
    VkPipelineLayout pipeline_layout_;
};

}// namespace rdmn::render

#endif//RDMN_VULKANGRAPHICSPIPELINE_HPP
