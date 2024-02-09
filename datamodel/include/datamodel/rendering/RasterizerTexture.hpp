#ifndef RCBE_ENGINE_RASTERIZERTEXTURE_HPP
#define RCBE_ENGINE_RASTERIZERTEXTURE_HPP

#include <memory>

#ifdef RDMN_VULKAN
#include <vulkan/vulkan.hpp>

#include <rcbe-engine/datamodel/rendering/vk_texture_types.hpp>
#endif

#include <rcbe-engine/datamodel/rendering/rasterizer_texture_types.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>

namespace rdmn::render {
class RasterizerTextureImplementation;

class RasterizerTexture : public std::enable_shared_from_this<RasterizerTexture> {
public:

    RasterizerTexture() = delete;
    RasterizerTexture(rasterizer_texture_config config, rcbe::visual::TexturePtr texture);
    ~RasterizerTexture();

    RasterizerTexture(const RasterizerTexture &other) = delete;
    RasterizerTexture &operator=(const RasterizerTexture &other) = delete;

    RasterizerTexture(RasterizerTexture &&other) noexcept;
    RasterizerTexture &operator=(RasterizerTexture &&other) noexcept;

    void bind(const size_t index = 0) const;
    void unbind() const;

    [[nodiscard]] bool deferred() const noexcept;

#ifdef RDMN_VULKAN
    /// TODO: remove when a proper way to supply to UBO is found @sckorn
    /// probably supply these objects to UBO, and init there
    [[nodiscard]] bool init(VkDevice logical_device,
                            VkPhysicalDevice physical_device,
                            VkCommandPool command_pool,
                            VkQueue graphics_queue);
    [[nodiscard]] bool init(VkDevice logical_device,
                            VkPhysicalDevice physical_device,
                            VkCommandPool command_pool,
                            VkQueue graphics_queue,
                            VkSampler global_sampler);
    [[nodiscard]] VkImageView getImageView() const;
    [[nodiscard]] VkSampler getImageSampler() const;
    [[nodiscard]] const rasterizer_texture_config &getConfig() const;
    [[nodiscard]] const rcbe::visual::TexturePtr &getVisualTexturePtr() const;
#endif

private:

    std::unique_ptr<RasterizerTextureImplementation> impl_;///TODO: find a way to get rid of this pImpl @sckorn
};

using RasterizerTexturePtr = std::shared_ptr<RasterizerTexture>;
using RasterizerTextureConstPtr = std::shared_ptr<const RasterizerTexture>;
}// namespace rdmn::render

#endif//RCBE_ENGINE_RASTERIZERTEXTURE_HPP
