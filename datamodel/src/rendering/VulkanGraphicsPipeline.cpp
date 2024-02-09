#include <rcbe-engine/datamodel/rendering/VulkanBufferObject.hpp>
#include <rcbe-engine/datamodel/rendering/VulkanGraphicsPipeline.hpp>

namespace {
using Material = rcbe::rendering::Material;
}

namespace rdmn::render {
VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
    vkDestroyPipeline(logical_device_, pipeline_, nullptr);
    vkDestroyPipelineLayout(logical_device_, pipeline_layout_, nullptr);
}

VulkanGraphicsPipeline::VulkanGraphicsPipeline(
    const Material &material,
    VkDevice logical_device,
    VkExtent2D extent,
    VkSampleCountFlagBits msaa_samples,
    VkDescriptorSetLayout desc_set_layout,
    VkRenderPass render_pass,
    size_t push_const_sizeof)
    : logical_device_ {logical_device} {
    const auto &vertex = material.getVertex();
    const auto &fragment = material.getFragment();

    if (!vertex || !fragment)
        throw std::runtime_error("No valid shaders supplied!");

    if (!vertex->moduleCreated())
        if (!vertex->createVkShaderModule(logical_device))
            throw std::runtime_error("Can't create vertex shader");

    if (!fragment->moduleCreated())
        if (!fragment->createVkShaderModule(logical_device))
            throw std::runtime_error("Can't create fragment shader");

    auto vert_module = vertex->handle();
    auto frag_module = fragment->handle();

    VkPipelineShaderStageCreateInfo vstage_create_info {};
    vstage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vstage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vstage_create_info.module = vert_module;
    vstage_create_info.pName = "main";

    VkPipelineShaderStageCreateInfo fstage_create_info {};
    fstage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fstage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fstage_create_info.module = frag_module;
    fstage_create_info.pName = "main";

    VkPipelineShaderStageCreateInfo create_infos[] = {
        vstage_create_info, fstage_create_info};

    auto binding_description = VulkanVertexBufferObject::getBindingDescription();
    auto attrib_descrs = VulkanVertexBufferObject::getAttributesDescription();
    VkVertexInputAttributeDescription attribute_descr[] = {attrib_descrs[0], attrib_descrs[2]};

    VkPipelineVertexInputStateCreateInfo vis_create_info {};
    vis_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vis_create_info.vertexBindingDescriptionCount = 1;
    vis_create_info.pVertexBindingDescriptions = std::addressof(binding_description);
    vis_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attrib_descrs.size());
    vis_create_info.pVertexAttributeDescriptions = attrib_descrs.data();

    VkPipelineInputAssemblyStateCreateInfo iasm_create_info {};
    iasm_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    iasm_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    iasm_create_info.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport {};
    viewport.x = .0f;
    viewport.y = .0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = .0f;
    viewport.maxDepth = 1.f;

    VkRect2D scissor {};
    scissor.offset = {0, 0};
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo vps_create_info {};
    vps_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vps_create_info.viewportCount = 1;
    vps_create_info.pViewports = std::addressof(viewport);
    vps_create_info.scissorCount = 1;
    vps_create_info.pScissors = std::addressof(scissor);

    VkPipelineRasterizationStateCreateInfo rs_create_info {};
    rs_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs_create_info.depthClampEnable = VK_FALSE;
    rs_create_info.rasterizerDiscardEnable = VK_FALSE;
    rs_create_info.polygonMode = VK_POLYGON_MODE_FILL;
    rs_create_info.lineWidth = 1.f;
    rs_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
    rs_create_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rs_create_info.depthBiasEnable = VK_FALSE;
    rs_create_info.depthBiasConstantFactor = .0f;
    rs_create_info.depthBiasClamp = .0f;
    rs_create_info.depthBiasSlopeFactor = 0.f;

    VkPipelineMultisampleStateCreateInfo ms_create_info {};
    ms_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms_create_info.rasterizationSamples = msaa_samples;
    ms_create_info.pSampleMask = nullptr;
    ms_create_info.alphaToCoverageEnable = VK_FALSE;
    ms_create_info.alphaToOneEnable = VK_FALSE;
    ms_create_info.sampleShadingEnable = VK_TRUE;
    ms_create_info.minSampleShading = 0.2f;

    VkPipelineColorBlendAttachmentState colorblend_attach_state {};
    colorblend_attach_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorblend_attach_state.blendEnable = VK_FALSE;
    colorblend_attach_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorblend_attach_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorblend_attach_state.colorBlendOp = VK_BLEND_OP_ADD;
    colorblend_attach_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorblend_attach_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorblend_attach_state.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo blending {};
    blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blending.logicOpEnable = VK_FALSE;
    blending.logicOp = VK_LOGIC_OP_COPY;
    blending.attachmentCount = 1;
    blending.pAttachments = std::addressof(colorblend_attach_state);
    blending.blendConstants[0] = 0.f;
    blending.blendConstants[1] = 0.f;
    blending.blendConstants[2] = 0.f;
    blending.blendConstants[3] = 0.f;

    VkPipelineLayoutCreateInfo pipeline_layout {};
    pipeline_layout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout.setLayoutCount = 1;
    pipeline_layout.pSetLayouts = std::addressof(desc_set_layout);

    VkPushConstantRange push_constant {};
    push_constant.offset = 0;
    push_constant.size = push_const_sizeof;
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    pipeline_layout.pushConstantRangeCount = 1;
    pipeline_layout.pPushConstantRanges = std::addressof(push_constant);

    auto res = vkCreatePipelineLayout(
        logical_device,
        std::addressof(pipeline_layout),
        nullptr,
        std::addressof(pipeline_layout_));
    if (res != VK_SUCCESS)
        throw std::runtime_error("Create pipeline layout!");

    VkPipelineDepthStencilStateCreateInfo depth_create_info {};
    depth_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_create_info.depthTestEnable = VK_TRUE;
    depth_create_info.depthWriteEnable = VK_TRUE;
    depth_create_info.depthCompareOp = VK_COMPARE_OP_LESS;
    depth_create_info.depthBoundsTestEnable = VK_FALSE;
    depth_create_info.minDepthBounds = 0.0f;
    depth_create_info.maxDepthBounds = 1.0f;
    depth_create_info.stencilTestEnable = VK_FALSE;
    depth_create_info.front = {};
    depth_create_info.back = {};

    VkGraphicsPipelineCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    create_info.stageCount = 2;
    create_info.pStages = create_infos;
    create_info.pVertexInputState = std::addressof(vis_create_info);
    create_info.pInputAssemblyState = std::addressof(iasm_create_info);
    create_info.pViewportState = std::addressof(vps_create_info);
    create_info.pRasterizationState = std::addressof(rs_create_info);
    create_info.pMultisampleState = std::addressof(ms_create_info);
    create_info.pDepthStencilState = std::addressof(depth_create_info);
    create_info.pColorBlendState = std::addressof(blending);
    create_info.pDynamicState = nullptr;
    create_info.layout = pipeline_layout_;
    create_info.renderPass = render_pass;
    create_info.subpass = 0;
    create_info.basePipelineHandle = VK_NULL_HANDLE;
    create_info.basePipelineIndex = -1;

    res = vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1, std::addressof(create_info), nullptr, std::addressof(pipeline_));
    if (res != VK_SUCCESS)
        throw std::runtime_error("Can't create pipeline");
}

const VkPipeline &VulkanGraphicsPipeline::getPipeline() const noexcept {
    return pipeline_;
}

const VkPipelineLayout &VulkanGraphicsPipeline::getLayout() const noexcept {
    return pipeline_layout_;
}
}// namespace rdmn::render
