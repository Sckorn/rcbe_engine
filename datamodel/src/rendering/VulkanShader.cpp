#include "VulkanShader.hpp"

#include <fstream>

namespace rdmn::render {
VulkanShader::VulkanShader(const rcbe::core::EnginePath &path, ShaderType type, ShaderState state)
    : type_(type)
    , state_(state)
    , shader_path_(path) {
    if (!std::filesystem::exists(shader_path_) || !std::filesystem::is_regular_file(path))
        throw std::runtime_error("Shader file doesn't exist or not a file!");

    if (state_ != ShaderState::precompiled)
        throw std::runtime_error("Vulkan shaders on the fly compilation is not supported yet!");

    std::ifstream ifs {shader_path_, std::ios::ate | std::ios::binary};
    if (!ifs)
        throw std::runtime_error("Can't open shader file for reading!");

    auto file_size = static_cast<size_t>(ifs.tellg());
    shader_bytes_.resize(file_size);

    ifs.seekg(0);
    ifs.read(shader_bytes_.data(), file_size);
}

VulkanShader::~VulkanShader() {
    if (shader_module_created_) {
        vkDestroyShaderModule(device_, shader_module_, nullptr);
    }
}

VkShaderModule VulkanShader::handle() const noexcept {
    return shader_module_;
}

const Shader::ShaderHandle &VulkanShader::compile() {
    throw std::runtime_error("On the fly Vulkan shader compilation is not supported yet!");
}

const rcbe::core::EnginePath &VulkanShader::path() const noexcept {
    return shader_path_;
}

bool VulkanShader::isCompiled() const noexcept {
    return true;/// TODO: fix this once on the fly compilation is implemented @sckorn, @radameon
}

ShaderType VulkanShader::type() const noexcept {
    return type_;
}

[[nodiscard]] bool VulkanShader::moduleCreated() const noexcept {
    return shader_module_created_;
}

bool VulkanShader::createVkShaderModule(VkDevice device) {
    VkShaderModuleCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = shader_bytes_.size();
    create_info.pCode = reinterpret_cast<const uint32_t *>(shader_bytes_.data());

    auto res = vkCreateShaderModule(device, std::addressof(create_info), nullptr, std::addressof(shader_module_));

    if (res != VK_SUCCESS) {
        return false;
    }

    shader_module_created_ = true;
    device_ = device;

    return true;
}

}// namespace rdmn::render
