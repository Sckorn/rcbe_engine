#ifndef RCBE_ENGINE_VULKANRASTERIZERTEXTURE_HPP
#define RCBE_ENGINE_VULKANRASTERIZERTEXTURE_HPP

#include <functional>

#include <vulkan/vulkan.h>

#include <rcbe-engine/datamodel/rendering/rasterizer_texture_helpers.hpp>
#include <rcbe-engine/datamodel/rendering/rasterizer_texture_types.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>

namespace rdmn::render {
class RasterizerTextureImplementation {
public:

    using DeferredTextureDeleter = std::function<void(VkImage, VkDeviceMemory, VkImageView, VkSampler)>;

    RasterizerTextureImplementation() = delete;
    RasterizerTextureImplementation(rasterizer_texture_config config, rcbe::visual::TexturePtr texture)
        : config_(config)
        , texture_(texture)
        , mip_levels_ {
              calculateMipLevels(texture_->getWidth(), texture_->getHeight())} {
    }

    RasterizerTextureImplementation(const RasterizerTextureImplementation &other) = delete;
    RasterizerTextureImplementation &operator=(const RasterizerTextureImplementation &other) = delete;

    ~RasterizerTextureImplementation() {
        if (deleter_) {
            deleter_(
                texture_image_,
                texture_image_memory_,
                texture_image_view_,
                texture_sampler_);
        } else {
            /// Should never be invoked under normal circumstances, e.g. if init was called and there is anything to
            /// deinit
            if (initialised_)
                BOOST_LOG_TRIVIAL(fatal) << "~RasterizerTextureImplementation no deleter available!!!";
            else
                BOOST_LOG_TRIVIAL(trace) << "Deleting non initialized Raster texture";
        }
    };

    void bind(const size_t index = 0) const {}/// TODO: fix this noop @sckorn, @radameon
    void unbind() const {}

    bool init(VkDevice logical_device,
              VkPhysicalDevice physical_device,
              VkCommandPool command_pool,
              VkQueue graphics_queue) {
        auto sampler_created = createTextureSampler(logical_device, texture_sampler_, mip_levels_);
        if (!sampler_created) {
            BOOST_LOG_TRIVIAL(error) << "Can't create texture sampler";
            return false;
        }

        deleter_ = DeferredTextureDeleter {
            [logical_device](VkImage texture_image,
                             VkDeviceMemory texture_image_memory,
                             VkImageView texture_image_view,
                             VkSampler texture_sampler) {
                vkDestroySampler(logical_device, texture_sampler, nullptr);
                vkDestroyImageView(logical_device, texture_image_view, nullptr);

                vkDestroyImage(logical_device, texture_image, nullptr);
                vkFreeMemory(logical_device, texture_image_memory, nullptr);
            }};

        initialised_ = true;

        return true;
    }

    bool init(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkCommandPool command_pool,
        VkQueue graphics_queue,
        VkSampler global_sampler) {
        const auto res = commonInitPreamble(logical_device, physical_device, command_pool, graphics_queue);
        if (!res)
            return false;

        texture_sampler_ = global_sampler;

        deleter_ = DeferredTextureDeleter {
            [logical_device](VkImage texture_image,
                             VkDeviceMemory texture_image_memory,
                             VkImageView texture_image_view,
                             VkSampler texture_sampler) {
                vkDestroyImageView(logical_device, texture_image_view, nullptr);

                vkDestroyImage(logical_device, texture_image, nullptr);
                vkFreeMemory(logical_device, texture_image_memory, nullptr);
            }};

        initialised_ = true;

        return true;
    }

    /// TODO: remove when a proper way to supply to UBO is found
    [[nodiscard]] VkImageView getImageView() const {
        return texture_image_view_;
    }

    [[nodiscard]] VkSampler getImageSampler() const {
        return texture_sampler_;
    }
    /// TODO: remove when a proper way to supply to UBO is found

    [[nodiscard]] bool deferred() const noexcept {
        return deferred_;
    }

    [[nodiscard]] const rasterizer_texture_config &getConfig() const {
        return config_;
    }

    [[nodiscard]] const rcbe::visual::TexturePtr &getVisualTexturePtr() const {
        return texture_;
    }

private:

    bool commonInitPreamble(
        VkDevice logical_device,
        VkPhysicalDevice physical_device,
        VkCommandPool command_pool,
        VkQueue graphics_queue) {
        {
            bool texture_image_created = createTextureImage(
                *texture_,
                mip_levels_,
                logical_device,
                physical_device,
                command_pool,
                graphics_queue,
                texture_image_,
                texture_image_memory_);
            if (!texture_image_created) {
                BOOST_LOG_TRIVIAL(error) << "Can't create texture image!";
                return false;
            }
        }

        {
            auto opt_img_view = createImageView(
                logical_device,
                texture_image_,
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_ASPECT_COLOR_BIT,
                mip_levels_);

            if (!opt_img_view) {
                BOOST_LOG_TRIVIAL(error) << "Can't create image view!";
                return false;
            }

            texture_image_view_ = *opt_img_view;
        }

        return true;
    }

    rasterizer_texture_config config_;
    rcbe::visual::TexturePtr texture_;
    const bool deferred_ = true;
    bool initialised_ = false;
    DeferredTextureDeleter deleter_;

    uint32_t mip_levels_;
    VkImage texture_image_;
    VkDeviceMemory texture_image_memory_;

    VkImageView texture_image_view_;
    /// Global sampler, owned by renderer, no need to destroy here
    VkSampler texture_sampler_ = VK_NULL_HANDLE;
};
}// namespace rdmn::render

#endif//RCBE_ENGINE_VULKANRASTERIZERTEXTURE_HPP
