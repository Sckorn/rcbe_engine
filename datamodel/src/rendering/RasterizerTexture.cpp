#include <rcbe-engine/datamodel/rendering/RasterizerTexture.hpp>

#ifdef RDMN_VULKAN
#include "VulkanRasterizerTexture.hpp"
#elif defined(RDMN_OPENGL)
#include "GLRasterizerTexture.hpp"
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
#else
static_assert(false, "Rasterizer backend is not set during build, this may signal a malformed build configuration!");
#endif

namespace rdmn::render {
RasterizerTexture::~RasterizerTexture() = default;

RasterizerTexture::RasterizerTexture(rasterizer_texture_config config, rcbe::visual::TexturePtr texture)
:
impl_(std::make_unique<RasterizerTextureImplementation>(config, texture)) {}

RasterizerTexture::RasterizerTexture(RasterizerTexture &&other) noexcept
:
impl_(std::move(other.impl_)) {
}

RasterizerTexture &RasterizerTexture::operator=(RasterizerTexture &&other) noexcept {
  if (!other.impl_)
    return *this;

  if (this == std::addressof(other))
	  return *this;

  impl_ = std::move(other.impl_);

  return *this;
}

void RasterizerTexture::bind(const size_t index) const {
    impl_->bind(index);
}

void RasterizerTexture::unbind() const {
    impl_->unbind();
}

bool RasterizerTexture::deferred() const noexcept {
    return impl_->deferred();
}

#ifdef RDMN_VULKAN
bool RasterizerTexture::init(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkCommandPool command_pool,
        VkQueue graphics_queue
) {
    return impl_->init(logical_device, physical_device, command_pool, graphics_queue);
}

bool RasterizerTexture::init(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkCommandPool command_pool,
        VkQueue graphics_queue,
        VkSampler global_sampler
) {
    return impl_->init(logical_device, physical_device, command_pool, graphics_queue, global_sampler);
}

VkImageView RasterizerTexture::getImageView() const {
    return impl_->getImageView();
}

VkSampler RasterizerTexture::getImageSampler() const {
    return impl_->getImageSampler();
}

const rasterizer_texture_config &RasterizerTexture::getConfig() const {
    return impl_->getConfig();
}

const rcbe::visual::TexturePtr &RasterizerTexture::getVisualTexturePtr() const {
    return impl_->getVisualTexturePtr();
}
#endif
}
