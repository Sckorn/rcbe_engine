#ifndef RDMN_ENGINE_VULKANRENDERER_HPP
#define RDMN_ENGINE_VULKANRENDERER_HPP

#ifdef __linux__
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <condition_variable>
#include <future>
#include <memory>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <rcbe-engine/datamodel/core/CoreObject.hpp>
#include <rcbe-engine/datamodel/geometry/Mesh.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/rendering/Material.hpp>
#include <rcbe-engine/datamodel/rendering/RasterizerTexture.hpp>
#include <rcbe-engine/datamodel/rendering/RenderingContext.hpp>
#include <rcbe-engine/datamodel/rendering/VulkanBufferObject.hpp>
#include <rcbe-engine/datamodel/rendering/VulkanGraphicsPipeline.hpp>
#include <rcbe-engine/datamodel/rendering/renderer_config.hpp>

namespace rdmn::render {

class R_PUBLIC_API VulkanRenderer {
public:

    VulkanRenderer() = delete;
    VulkanRenderer(
        rcbe::rendering::renderer_config &&config,
        const std::shared_ptr<rcbe::rendering::RenderingContext> &context);

    ~VulkanRenderer();

    [[nodiscard]] bool running() const;

    [[nodiscard]] const rcbe::rendering::renderer_config &getConfig() const noexcept;

    decltype(auto) startAsync() {
        return std::async(std::launch::async, [this]() {
            start();
        });
    }
    void start();
    void stop();
    bool addObject(rcbe::core::CoreObject &&renderer_object);
    bool addObjects(std::vector<rcbe::core::CoreObject> &&renderer_objects);

    void reshape();

    void onStop(rcbe::rendering::RendererStopHandlerType &&handler);
    void waitRendererReady();

private:

    struct queue_family_indices {
        std::optional<uint32_t> graphics_family = std::nullopt;
        std::optional<uint32_t> presentation_family = std::nullopt;

        [[nodiscard]] bool indexSet() const {
            return graphics_family.has_value() && presentation_family.has_value();
        }
    };

    struct swap_chain_support_details {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> modes;
        bool obtained_successfully = false;
    };

    struct image_view_creation_result {
        bool success = false;
        size_t failed_index;
    };

    using OptionalExtensionFallback = std::function<bool()>;
    struct optional_ext_req {
        std::string_view extension_name;
        OptionalExtensionFallback fallback;
    };

    struct predef_objs_load_res {
        std::vector<rcbe::geometry::Mesh> meshes;
        std::vector<VkImageView> rasterizer_tex_image_views;
        std::vector<VkSampler> rasterizer_tex_samplers;
        bool success = false;
    };

    using MaterialPtr = std::shared_ptr<rcbe::core::CoreObject>;
    using GameObjects = std::vector<rcbe::core::CoreObject>;

    void mainLoop();
    void renderFrame();
    bool createVulkanInstance(VkInstance &instance);
    bool listAndCheckExtensions(std::vector<std::string> required = {}, std::vector<optional_ext_req> optional = {});
    bool listAndCheckLayers(const std::vector<const char *> &to_check = {});
    bool selectPhysicalDevice(VkPhysicalDevice &device);
    bool deviceSupported(VkPhysicalDevice device);
    queue_family_indices getQueueFamilyIndices(VkPhysicalDevice device);
    bool createLogicalDevice(VkDevice &logical_device);
    bool createSurface();
#ifdef __linux__
    bool createXlibSurface();
#endif
#ifdef _WIN32
    bool createWin32Surface();
#endif
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    swap_chain_support_details querySwapChainSupportDetails(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &available_formats);
    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &available_pres_modes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    bool createSwapChain(VkPhysicalDevice device);
    image_view_creation_result createSwapChainViews();
    bool createGraphicsPipeline(VkDevice logical_device);
    bool createRenderPass(VkDevice logical_device);
    bool createFramebuffers(VkDevice logcal_device);
    bool createCommandPool(VkDevice logical_device);
    bool createCommandBuffers(VkDevice logical_device);
    bool createSyncObjects(VkDevice logical_device);
    bool recreateSwapchain(VkPhysicalDevice device, VkDevice logical_device, bool objects_added, bool resized);
    void cleanupSwapchain(bool final_cleanup = true);
    bool createDescriptorSetLayout(VkDevice logical_device);
    VkSampleCountFlagBits getMaxUsableSamplesCount();
    bool createColorResources(VkDevice logical_device);
    predef_objs_load_res loadPredefObjs(bool secondary_invokation, bool added_object = false, bool resized = false);
    void repopulateTextureIndices();
    bool initMaterialTextures(const rcbe::rendering::Material &mat);
    bool createDepthResources(VkDevice logical_device);
    std::optional<VkFormat> findSupportedFormat(
        const std::vector<VkFormat> &candidates,
        VkPhysicalDevice physical_device,
        VkImageTiling tiling,
        VkFormatFeatureFlags features);
    bool hasStencilComponent(VkFormat format);
    void handleRenderedObject(
        rcbe::core::CoreObject &&renderer_object,
        std::unordered_map<MaterialPtr, GameObjects> &objects,
        std::unordered_set<MaterialPtr> &material_cache,
        size_t reserve_for = 1);

    /// Depth buffer
    VkImage depth_image_;
    VkDeviceMemory depth_image_memory_;
    VkImageView depth_image_view_;
    /// Depth buffer

    bool ready_ = false;
    std::atomic_bool running_ = false;
    std::atomic_bool added_object_ = false;
    rcbe::rendering::renderer_config config_;
    std::shared_ptr<rcbe::rendering::RenderingContext> context_;
    bool use_global_sampler_ = false;

    VkInstance instance_;
    bool instance_created_ = false;
    VkPhysicalDevice device_ = VK_NULL_HANDLE;
    VkDevice logical_device_;
    VkQueue graphics_queue_;
    VkQueue presentation_queue_;
    VkSurfaceKHR surface_;
    VkSwapchainKHR swap_chain_;

    std::vector<VkImage> swap_chain_images_;
    std::vector<VkImageView> swap_chain_views_;

    VkFormat swap_chain_format_;
    VkExtent2D swap_chain_extent_;

    VkRenderPass render_pass_;
    VkDescriptorSetLayout descriptor_set_layout_;

    // material to pipeline
    std::unordered_map<MaterialPtr, std::shared_ptr<VulkanGraphicsPipeline>> pipelines_;

    std::vector<VkFramebuffer> swap_chain_framebuffers_;

    VkCommandPool command_pool_;
    std::vector<VkCommandBuffer> command_buffers_;

    std::vector<VkSemaphore> image_available_;
    std::vector<VkSemaphore> render_finished_;
    std::vector<VkFence> inflight_fences_;
    std::vector<VkFence> images_inflight_;
    size_t current_frame_ = 0;

    VkSampleCountFlagBits msaa_samples_ = VK_SAMPLE_COUNT_1_BIT;

    /// MSAA rendering target

    VkImage color_image_;
    VkDeviceMemory color_image_memory_;
    VkImageView color_image_view_;

    /// MSAA rendering target

    std::atomic_bool resized_ = false;

    std::shared_ptr<VulkanVertexBufferObject> buffer_object_ = nullptr;
    std::shared_ptr<VulkanUniformBufferObject> uniform_buffer_object_ = nullptr;

    std::unordered_map<MaterialPtr, GameObjects> objects_;

    std::unordered_map<rcbe::visual::TexturePtr, size_t> texture_to_index_;  /// rasterizer texture to index in desc set
    std::unordered_map<size_t, std::vector<size_t>> object_to_sampler_index_;/// Id to sampler index

    std::unordered_set<MaterialPtr> material_cache_;

    VkSampler rasterizer_tex_global_sampler_ = VK_NULL_HANDLE;

    std::mutex waiting_mutex_;
    std::condition_variable waiting_renderer_cv_;

    [[maybe_unused]] rcbe::rendering::RendererStopHandlerType stop_handler_;

    const std::vector<const char *> validation_layers_ = {
        "VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> device_extensions_ = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

using VulkanRendererPtr = std::unique_ptr<VulkanRenderer>;
using VulkanRendererConstPtr = std::unique_ptr<const VulkanRenderer>;
}// namespace rdmn::render

#endif//RDMN_ENGINE_VULKANRENDERER_HPP
