#include <rcbe-engine/fundamentals/convinience.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>

#include <boost/log/trivial.hpp>

#ifdef RDMN_VULKAN
#include "VulkanShader.hpp"
#elif defined(RDMN_OPENGL)
#include "GLShader.hpp"
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
/// TODO: fill when DirectX rendering is implemented
#else
static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif

namespace rdmn::render {

class ShaderImpl {
public:
    ShaderImpl() = delete;
    ~ShaderImpl() = default;

    ShaderImpl(const ShaderImpl &other) = delete;
    ShaderImpl &operator=(const ShaderImpl &other) = delete;

    ShaderImpl(ShaderImpl &&other);
    ShaderImpl &operator=(ShaderImpl &&other);

    ShaderImpl(const rcbe::core::EnginePath &path, ShaderType type, ShaderState state = ShaderState::source);

    [[nodiscard]] const Shader::ShaderHandle &compile();
    [[nodiscard]] const rcbe::core::EnginePath &path() const noexcept;
    [[nodiscard]] bool isCompiled() const noexcept;
    [[nodiscard]] ShaderType type() const noexcept;

#ifdef RDMN_VULKAN
    [[nodiscard]] Shader::ShaderHandle handle() const noexcept;
    [[nodiscard]] bool createVkShaderModule(VkDevice device);
    [[nodiscard]] bool moduleCreated() const noexcept;
#elif defined(RDMN_OPENGL)
    [[nodiscard]] const Shader::ShaderHandle &handle() const noexcept;
    [[nodiscard]] int maxVertexAttribs() const noexcept;
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    /// TODO: fill when DirectX rendering is implemented
#else
    static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif
private:
#ifdef RDMN_VULKAN
    VulkanShader shader_;
#elif defined(RDMN_OPENGL)
    GLShader shader_;
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
/// TODO: fill when DirectX rendering is implemented
#else
    static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif
};

ShaderImpl::ShaderImpl(ShaderImpl &&other) = default;
ShaderImpl &ShaderImpl::operator=(ShaderImpl &&other) = default;

ShaderImpl::ShaderImpl(const rcbe::core::EnginePath &path, ShaderType type, ShaderState state)
: shader_(path, type, state) {
}

#ifdef RDMN_VULKAN
Shader::ShaderHandle ShaderImpl::handle() const noexcept {
    return shader_.handle();
}

bool ShaderImpl::createVkShaderModule(VkDevice device) {
    return shader_.createVkShaderModule(device);
}

bool ShaderImpl::moduleCreated() const noexcept {
  return shader_.moduleCreated();
}
#elif defined(RDMN_OPENGL)
const Shader::ShaderHandle &ShaderImpl::handle() const noexcept {
    return shader_.handle();
}

int ShaderImpl::maxVertexAttribs() const noexcept {
    return shader_.maxVertexAttribs();
}
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
/// TODO: fill when DirectX rendering is implemented
#else
static_assert(false, "Rasterizer backend not selected, build configuration malformed!");
#endif

const Shader::ShaderHandle &ShaderImpl::compile() {
    return shader_.compile();
}

const rcbe::core::EnginePath &ShaderImpl::path() const noexcept {
    return shader_.path();
}

bool ShaderImpl::isCompiled() const noexcept {
    return shader_.isCompiled();
}

ShaderType ShaderImpl::type() const noexcept {
    return shader_.type();
}

Shader::~Shader() = default;

Shader::Shader(Shader &&other) noexcept = default;
Shader &Shader::operator=(Shader &&other) noexcept = default;

Shader::Shader(const rcbe::core::EnginePath &path, ShaderType type, ShaderState state)
: impl_(std::make_unique<ShaderImpl>(path, type, state)) {

}

const Shader::ShaderHandle &Shader::compile() {
    return impl_->compile();
}

const rcbe::core::EnginePath &Shader::path() const noexcept {
    return impl_->path();
}

bool Shader::isCompiled() const noexcept {
    return impl_->isCompiled();
}

ShaderType Shader::type() const noexcept {
    return impl_->type();
}

#ifdef RDMN_VULKAN
Shader::ShaderHandle Shader::handle() const noexcept {
    return impl_->handle();
}

bool Shader::createVkShaderModule(VkDevice device) {
    return impl_->createVkShaderModule(device);
}

bool Shader::moduleCreated() const noexcept {
  return impl_->moduleCreated();
}
#elif defined(RDMN_OPENGL)
int Shader::maxVertexAttribs() const noexcept {
    return impl_->maxVertexAttribs();
}

const Shader::ShaderHandle &Shader::handle() const noexcept {
    return impl_->handle();
}
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
/// TODO: fill when DirectX rendering is implemented
#else
static_assert(false, "Rasterizer backend not selected, build configuration malformed!");
#endif

}
