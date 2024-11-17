#ifndef RDMN_ENGINE_SHADER_HPP
#define RDMN_ENGINE_SHADER_HPP

#include <filesystem>
#include <memory>

#include <rcbe-engine/fundamentals/convinience.hpp>
#include <rcbe-engine/fundamentals/types.hpp>

#if defined(RDMN_VULKAN)
#include <vulkan/vulkan.hpp>
#elif defined(RDMN_OPENGL)
#include <GL/gl.h>
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
/// TODO: fill when DirectX rendering is implemented
#else
static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif

namespace rdmn::render {
static constexpr size_t SHADER_ERROR_MSG_SIZE = 512;

enum class ShaderState {
    source,
    precompiled,
    unknown = -1,
};

enum class ShaderType {
#if defined(RDMN_VULKAN)
    fragment,
    vertex,
#elif defined(RDMN_OPENGL)
    fragment = GL_FRAGMENT_SHADER,
    vertex = GL_VERTEX_SHADER,
    geometry = GL_GEOMETRY_SHADER,
    tesselation_evaluation = GL_TESS_EVALUATION_SHADER,
    tesselation_control = GL_TESS_CONTROL_SHADER,
    compute = GL_COMPUTE_SHADER,
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
/// TODO: fill when DirectX rendering is implemented
#else
    static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif
    unknown = -1,
};

class ShaderImpl;

class R_PUBLIC_API Shader {
public:

#ifdef RDMN_VULKAN
    using ShaderHandle = VkShaderModule;
#elif defined(RDMN_OPENGL)
    using ShaderHandle = size_t;
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    /// TODO: fill when DirectX rendering is implemented
#else
    static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif

    Shader() = delete;
    ~Shader();

    Shader(const rcbe::core::EnginePath &path, ShaderType type, ShaderState state = ShaderState::source);

    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

    [[nodiscard]] const ShaderHandle &compile();
    [[nodiscard]] const rcbe::core::EnginePath &path() const noexcept;
    [[nodiscard]] bool isCompiled() const noexcept;
    [[nodiscard]] ShaderType type() const noexcept;

#ifdef RDMN_VULKAN
    [[nodiscard]] ShaderHandle handle() const noexcept;
    [[nodiscard]] bool moduleCreated() const noexcept;
    [[nodiscard]] bool createVkShaderModule(VkDevice device);
#elif defined(RDMN_OPENGL)
    [[nodiscard]] int maxVertexAttribs() const noexcept;
    [[nodiscard]] const ShaderHandle &handle() const noexcept;
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    /// TODO: fill when DirectX rendering is implemented
#else
    static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif

private:

    std::shared_ptr<ShaderImpl> impl_;///TODO: find a way to get rid of the pImpl @sckorn
};

}// namespace rdmn::render

#endif// RDMN_ENGINE_SHADER_HPP
