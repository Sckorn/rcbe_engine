#ifndef RDMN_ENGINE_VULKANSHADER_HPP
#define RDMN_ENGINE_VULKANSHADER_HPP

#include <vector>

#include <vulkan/vulkan.hpp>

#include <rcbe-engine/datamodel/rendering/Shader.hpp>

namespace rdmn::render {
class VulkanShader {
public:

    VulkanShader(const rcbe::core::EnginePath &path, ShaderType type, ShaderState state = ShaderState::source);
    ~VulkanShader();

    VulkanShader(const VulkanShader &other) = delete;
    VulkanShader &operator=(const VulkanShader &other) = delete;

    VulkanShader(VulkanShader &&other) noexcept = default;
    VulkanShader &operator=(VulkanShader &&other) noexcept = default;

    [[nodiscard]] VkShaderModule handle() const noexcept;
    [[nodiscard]] const Shader::ShaderHandle &compile();
    [[nodiscard]] const rcbe::core::EnginePath &path() const noexcept;
    [[nodiscard]] bool isCompiled() const noexcept;
    [[nodiscard]] ShaderType type() const noexcept;
    [[nodiscard]] bool moduleCreated() const noexcept;

    [[nodiscard]] bool createVkShaderModule(VkDevice device);

public:

    ShaderType type_ = ShaderType::unknown;
    ShaderState state_ = ShaderState::unknown;
    rcbe::core::EnginePath shader_path_;
    std::vector<char> shader_bytes_;

    VkDevice device_;
    VkShaderModule shader_module_;
    bool shader_module_created_ = false;
};
}// namespace rdmn::render

#endif//RDMN_ENGINE_VULKANSHADER_HPP
