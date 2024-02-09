#include <variant>

#include <rcbe-engine/exceptions/not_implemented.hpp>
#include <rcbe-engine/fundamentals/convinience.hpp>
#include <rcbe-engine/renderer/Renderer.hpp>

#ifdef RDMN_VULKAN
#include "VulkanRenderer.hpp"
#elif defined(RDMN_OPENGL)
#include "GLRenderer.hpp"
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
/// TODO: fill when DirectX rendering is implemented
#else
static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif

namespace rdmn::render {
class RendererImpl {
public:

    RendererImpl() = delete;
    RendererImpl(rcbe::rendering::renderer_config &&config, const std::shared_ptr<rcbe::rendering::RenderingContext> &context);
    ~RendererImpl() = default;

    [[nodiscard]] bool running() const;

    void start();
    void stop();

    [[nodiscard]] const rcbe::rendering::renderer_config &getConfig() const noexcept;

    void reshape();

    void addObject(rcbe::core::CoreObject &&object);
    void addObjects(std::vector<rcbe::core::CoreObject> &&object);

    void waitRendererReady();

private:

#ifdef RDMN_VULKAN
    VulkanRendererPtr impl_;
#elif defined(RDMN_OPENGL)
    GLRendererPtr impl_;
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    /// TODO: fill when DirectX rendering is implemented
#else
    static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif
};

bool RendererImpl::running() const {
    return impl_->running();
}

void RendererImpl::start() {
    impl_->start();
}

void RendererImpl::stop() {
    impl_->stop();
}

const rcbe::rendering::renderer_config &RendererImpl::getConfig() const noexcept {
    return impl_->getConfig();
}

void RendererImpl::reshape() {
    impl_->reshape();
}

void RendererImpl::addObject(rcbe::core::CoreObject &&object) {
    impl_->addObject(std::move(object));
}

void RendererImpl::addObjects(std::vector<rcbe::core::CoreObject> &&object) {
#ifdef RDMN_VULKAN
    impl_->addObjects(std::move(object));
#elif RDMN_OPENGL
    /// TODO: fix this somehow @sckorn
    throw rcbe::exception::NotImplementedException("Adding multiple objects is not supported for OpenGL renderer!");
#endif
}

void RendererImpl::waitRendererReady() {
    impl_->waitRendererReady();
}

RendererImpl::RendererImpl(
    rcbe::rendering::renderer_config &&config,
    const std::shared_ptr<rcbe::rendering::RenderingContext> &context
#ifdef RDMN_VULKAN
    )
    : impl_ {std::make_unique<VulkanRenderer>(std::move(config), context)} {
#elif defined(RDMN_OPENGL)
    )
    : impl_ {std::make_unique<GLRenderer>(std::move(config), context)} {
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
    /// TODO: fill when DirectX rendering is implemented
) {
#else
) {
    static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif
}

Renderer::~Renderer() = default;

Renderer::Renderer(
    rcbe::rendering::renderer_config &&config,
    const std::shared_ptr<rcbe::rendering::RenderingContext> &context)
    : impl_(std::make_unique<RendererImpl>(std::move(config), context)) {
}

bool Renderer::running() const {
    return impl_->running();
}

void Renderer::waitRendererReady() {
    impl_->waitRendererReady();
}

void Renderer::start() {
    impl_->start();
}

void Renderer::stop() {
    impl_->stop();
}

const rcbe::rendering::renderer_config &Renderer::getConfig() const noexcept {
    return impl_->getConfig();
}

void Renderer::addObject(rcbe::core::CoreObject &&object) {
    impl_->addObject(std::move(object));
}

void Renderer::addObjects(std::vector<rcbe::core::CoreObject> &&object) {
#ifdef RDMN_VULKAN
    impl_->addObjects(std::move(object));
#elif RDMN_OPENGL
    /// TODO: fix this somehow @sckorn
    throw rcbe::exception::NotImplementedException("Adding multiple objects is not supported for OpenGL renderer!");
#endif
}

void Renderer::reshape() {
    impl_->reshape();
}

RendererPtr make_renderer_ptr(
    rcbe::rendering::renderer_config &&config,
    const std::shared_ptr<rcbe::rendering::RenderingContext> &context) {
    return std::make_unique<Renderer>(std::move(config), context);
}

RendererConstPtr make_renderer_const_ptr(
    rcbe::rendering::renderer_config &&config,
    const std::shared_ptr<rcbe::rendering::RenderingContext> &context) {
    return std::make_unique<const Renderer>(std::move(config), context);
}
}// namespace rdmn::render
