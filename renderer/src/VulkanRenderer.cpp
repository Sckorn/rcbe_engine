#define VK_USE_PLATFORM_XLIB_KHR

#include "VulkanRenderer.hpp"

#include <algorithm>
#include <exception>
#include <limits>
#include <vector>

#include <vulkan/vulkan_xlib.h>

#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/math/matrix_helpers.hpp>
#include <rcbe-engine/datamodel/rendering/Material.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/datamodel/rendering/matrix_helpers.hpp>
#include <rcbe-engine/datamodel/rendering/rasterizer_texture_helpers.hpp>
#include <rcbe-engine/engine_config.hpp>
#include <rcbe-engine/parsers/tga/tga_parser.hpp>
#include <rcbe-engine/parsers/x3d/x3d_parser.hpp>

inline constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;
inline constexpr size_t TOTAL_VK_EXTENSIONS = 6;
inline constexpr size_t TOTAL_SHADER_TEXTURES = 8;
inline constexpr float NEAR_PLANE_COORDINATE = 0.1;
inline constexpr float FAR_PLANE_COORDINATE = 10.0;
inline constexpr const char *APPLICATION_NAME = "RDMN Renderer";

namespace {
struct MeshPushConstants {
    rcbe::math::Matrix4x4f model_matrix;
    uint32_t sampler_index;
    uint32_t sampler_secondary_index = 0;
};

std::pair<std::vector<VkSampler>, std::vector<VkImageView>> repopulateSamplersAndViews(
    std::unordered_map<rcbe::visual::TexturePtr, size_t> &texture_to_index,
    const std::unordered_set<std::shared_ptr<rcbe::core::CoreObject>> &material_cache,
    const bool use_global_sampler) {
    std::pair<std::vector<VkSampler>, std::vector<VkImageView>> ret {};
    auto &rasterizer_tex_samplers = ret.first;
    auto &rasterizer_tex_image_views = ret.second;

    for (const auto &m : material_cache) {
        const auto &material = m->as<rcbe::rendering::Material>();
        rasterizer_tex_samplers.reserve(material.getTextures().size());
        rasterizer_tex_image_views.reserve(material.getTextures().size());
        for (const auto &[vtex, rtex] : material.getTextures()) {
            if (!use_global_sampler) {
                rasterizer_tex_samplers.push_back(rtex->getImageSampler());
            }
            texture_to_index[vtex] = rasterizer_tex_image_views.size();
            rasterizer_tex_image_views.push_back(rtex->getImageView());
        }
    }

    return ret;
}

rcbe::core::CoreObject loadCorner() {
    rcbe::visual::texture_config tex_conf;
    tex_conf.component_order = decltype(tex_conf.component_order)::RGBA;

    rdmn::render::rasterizer_texture_config raster_tex_conf;
    raster_tex_conf.filtering_type = decltype(raster_tex_conf.filtering_type)::linear;
    raster_tex_conf.texture_type = decltype(raster_tex_conf.texture_type)::texture_2d;
    raster_tex_conf.wrapping_type = decltype(raster_tex_conf.wrapping_type)::repeat;
    raster_tex_conf.image_config = tex_conf;

    std::vector<rcbe::rendering::Material::ShaderArguments> shader_args;
    shader_args.reserve(2);
    shader_args.push_back(
        {"datamodel/data/rendering/shaders/default_vulkan.vert.spv",
         rdmn::render::ShaderType::vertex,
         rdmn::render::ShaderState::precompiled});
    shader_args.push_back(
        {"datamodel/data/rendering/shaders/wolf_vulkan.frag.spv",
         rdmn::render::ShaderType::fragment,
         rdmn::render::ShaderState::precompiled});

    rcbe::rendering::Material::MaterialConfig config {
        std::move(shader_args),
        {}};
    rcbe::rendering::Material material(
        std::move(config));

    auto corner = rcbe::parsers::parse_meshes("parsers/test/resources/simple_edge_quad.x3d")[0];
    rcbe::math::Transform trn {
        rcbe::math::Matrix4x4 {
            0.0, 0., 1., 0.,
            0., 1., 0., 1.5,
            -1., 0., 0., 0.,
            0., 0., 0., 1.}};
    corner.transform(trn);
    std::string name = "corner";
    rcbe::core::CoreObject co {std::move(name)};
    co.addComponent<rcbe::geometry::Mesh>(std::move(corner));
    co.addComponent<rcbe::rendering::Material>(std::move(material));

    return co;
}
}// namespace

namespace rdmn::render {

VulkanRenderer::VulkanRenderer(
    rcbe::rendering::renderer_config &&config,
    const std::shared_ptr<rcbe::rendering::RenderingContext> &context)
    : config_(std::move(config))
    , context_(context) {
    {
        auto instance_created = createVulkanInstance(instance_);
        if (!instance_created) {
            throw std::runtime_error("VK Instance couldn't be created!");
        }
    }

    {
        auto surface_created = createSurface();
        if (!surface_created)
            throw std::runtime_error("Can't create window surface for rendering");
    }

    {
        auto listed_extensions = listAndCheckExtensions(
            {},
            {OptionalExtensionRequest {
                std::string_view {VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME},
                [this]() { return (use_global_sampler_ = true); }}});
        if (!listed_extensions)
            throw std::runtime_error("Unable to list extensions, or extension is missing");
    }

    {
        auto checked_layers = listAndCheckLayers(validation_layers_);
        if (!checked_layers)
            throw std::runtime_error("Unable to get layers or layer unsupported!");
    }

    {
        auto phys_device_selected = selectPhysicalDevice(device_);
        if (!phys_device_selected)
            throw std::runtime_error("Can't find a suitable physical device!");
    }

    {
        auto logical_device_created = createLogicalDevice(logical_device_);
        if (!logical_device_created)
            throw std::runtime_error("Can't create logical device!");
    }

    {
        auto swap_chain_created = createSwapChain(device_);
        if (!swap_chain_created)
            throw std::runtime_error("Can't create swap chain!");
    }

    {
        auto image_views_created = createSwapChainViews();
        if (!image_views_created.success) {
            for (size_t i = 0; i < image_views_created.failed_index; ++i) {
                vkDestroyImageView(logical_device_, swap_chain_views_[i], nullptr);
            }
            throw std::runtime_error("Couldn't create swap chain image views!");
        }
    }

    {
        auto render_pass_created = createRenderPass(logical_device_);
        if (!render_pass_created)
            throw std::runtime_error("Can't create render pass!");
    }

    {
        auto descriptor_set_layout_created = createDescriptorSetLayout(logical_device_);
        if (!descriptor_set_layout_created)
            throw std::runtime_error("Can't create descriptor set layout!");
    }

    preexistant_objects_load_result preexistent_objects_data {};
    {
        preexistent_objects_data = loadPreexistentObjects();
        if (!preexistent_objects_data.success)
            throw std::runtime_error("Can't load preexistent objects into scene!");
    }

    {
        auto graph_pipeline_created = createGraphicsPipeline(logical_device_);
        if (!graph_pipeline_created)
            throw std::runtime_error("Can't create graphics pipeline!");
    }

    /// Color buffer
    {
        auto color_buffer_created = createColorResources(logical_device_);
        if (!color_buffer_created)
            throw std::runtime_error("Can't create color resources!");
    }
    /// Color buffer

    /// Depth buffer
    {
        auto depth_buffer_created = createDepthResources(logical_device_);
        if (!depth_buffer_created)
            throw std::runtime_error("Can't create depth resources");
    }
    /// Depth buffer

    {
        auto framebuffers_created = createFramebuffers(logical_device_);
        if (!framebuffers_created)
            throw std::runtime_error("Can't create framebuffers!");
    }

    {
        auto command_pool_created = createCommandPool(logical_device_);
        if (!command_pool_created)
            throw std::runtime_error("Can't create command pool!");
    }

    /// Replace with textureObject creation when it is done
    {
        auto res = initPreexistentTextures(preexistent_objects_data);
        if (!res)
            throw std::runtime_error("Can't init preexistent textures!");
    }

    auto &[meshes, rasterizer_tex_image_views, rasterizer_tex_samplers, _] = preexistent_objects_data;

    rasterizer_tex_image_views.clear();
    rasterizer_tex_samplers.clear();
    std::tie(rasterizer_tex_samplers, rasterizer_tex_image_views) =
        repopulateSamplersAndViews(texture_to_index_, material_cache_, use_global_sampler_);

    repopulateTextureIndices();

    buffer_object_ = std::make_shared<VulkanVertexBufferObject>(
        meshes,
        logical_device_,
        device_,
        presentation_queue_,
        command_pool_);

    if (use_global_sampler_) {
        uniform_buffer_object_ = std::make_shared<VulkanUniformBufferObject>(
            logical_device_,
            device_,
            descriptor_set_layout_,
            rasterizer_tex_global_sampler_,
            std::move(rasterizer_tex_image_views),
            TOTAL_SHADER_TEXTURES,
            swap_chain_images_.size());
    } else {
        uniform_buffer_object_ = std::make_shared<VulkanUniformBufferObject>(
            logical_device_,
            device_,
            descriptor_set_layout_,
            std::move(rasterizer_tex_image_views),
            std::move(rasterizer_tex_samplers),
            swap_chain_images_.size());
    }

    {
        auto command_buffers_created = createCommandBuffers(logical_device_);
        if (!command_buffers_created)
            throw std::runtime_error("Can't create command buffers");
    }

    {
        auto semaphores_created = createSyncObjects(logical_device_);
        if (!semaphores_created)
            throw std::runtime_error("Can't create semaphores");
    }

    ready_ = true;
    waiting_renderer_cv_.notify_one();
}

VulkanRenderer::~VulkanRenderer() {
    cleanupSwapchain(true);

    vkDestroyDescriptorSetLayout(logical_device_, descriptor_set_layout_, nullptr);

    // delete buffer here
    if (buffer_object_) {
        buffer_object_->deleteBuffer();
        if (buffer_object_.use_count() == 1) {
            buffer_object_.reset();
        }
    }

    for (auto &[mat_ptr, obj_col] : objects_) {
        obj_col.clear();
    }
    objects_.clear();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroySemaphore(logical_device_, render_finished_[i], nullptr);
        vkDestroySemaphore(logical_device_, image_available_[i], nullptr);
        vkDestroyFence(logical_device_, inflight_fences_[i], nullptr);
    }

    vkDestroyCommandPool(logical_device_, command_pool_, nullptr);

    vkDestroySurfaceKHR(instance_, surface_, nullptr);
    vkDestroyDevice(logical_device_, nullptr);
    if (instance_created_)
        vkDestroyInstance(instance_, nullptr);
}

void VulkanRenderer::cleanupSwapchain(bool final_cleanup) {
    vkDestroyImageView(logical_device_, color_image_view_, nullptr);
    vkDestroyImage(logical_device_, color_image_, nullptr);
    vkFreeMemory(logical_device_, color_image_memory_, nullptr);

    vkDestroyImageView(logical_device_, depth_image_view_, nullptr);
    vkDestroyImage(logical_device_, depth_image_, nullptr);
    vkFreeMemory(logical_device_, depth_image_memory_, nullptr);

    for (size_t i = 0; i < swap_chain_framebuffers_.size(); ++i) {
        vkDestroyFramebuffer(logical_device_, swap_chain_framebuffers_[i], nullptr);
    }

    vkFreeCommandBuffers(
        logical_device_,
        command_pool_,
        static_cast<uint32_t>(command_buffers_.size()),
        command_buffers_.data());

    command_buffers_.clear();

    if (final_cleanup) {
        for (auto &m : material_cache_) {
            auto &mat = m->as<rcbe::rendering::Material>();
            mat.getTextures().clear();
        }

        for (auto &m : material_cache_) {
            auto &material = m->as<rcbe::rendering::Material>();

            material.getVertex().reset();
            material.getFragment().reset();
        }
    }

    material_cache_.clear();

    if (use_global_sampler_) {
        vkDestroySampler(logical_device_, rasterizer_tex_global_sampler_, nullptr);
    }

    pipelines_.clear();

    vkDestroyRenderPass(logical_device_, render_pass_, nullptr);

    for (size_t i = 0; i < swap_chain_views_.size(); ++i) {
        vkDestroyImageView(logical_device_, swap_chain_views_[i], nullptr);
    }

    vkDestroySwapchainKHR(logical_device_, swap_chain_, nullptr);

    if (uniform_buffer_object_) {
        uniform_buffer_object_->deleteBuffer();
        if (uniform_buffer_object_.use_count() == 1)
            uniform_buffer_object_.reset();
    }
}

bool VulkanRenderer::running() const {
    return running_.load();
}

const rcbe::rendering::renderer_config &VulkanRenderer::getConfig() const noexcept {
    return config_;
}

void VulkanRenderer::start() {
    bool curr = running_.load();
    if (!running_.compare_exchange_strong(curr, true)) {
        BOOST_LOG_TRIVIAL(warning) << "Renderer already running!";
        return;
    }

    context_->startTime(std::chrono::steady_clock::now());

    mainLoop();
}

void VulkanRenderer::stop() {
    bool curr = running_.load();
    if (!running_.compare_exchange_strong(curr, false))
        BOOST_LOG_TRIVIAL(warning) << "Renderer already stopped!";
}

bool VulkanRenderer::addObject(rcbe::core::CoreObject &&renderer_object) {
    if (!renderer_object.hasComponent<rcbe::rendering::Material>()) {
        BOOST_LOG_TRIVIAL(trace) << "Object doesn't have material, should at least have default!";
        return false;
    }

    handleRenderedObject(renderer_object, objects_, material_cache_);

    bool object_added_flag = added_object_.load();
    while (!added_object_.compare_exchange_strong(object_added_flag, true))
        ;

    return true;
}

bool VulkanRenderer::addObjects(std::vector<rcbe::core::CoreObject> &&renderer_objects) {
    for (auto &&ro : renderer_objects) {
        if (!ro.hasComponent<rcbe::rendering::Material>()) {
            BOOST_LOG_TRIVIAL(trace) << "Object doesn't have material, should at least have default! Skipping " << ro.name();
            continue;
        }

        handleRenderedObject(ro, objects_, material_cache_);
    }

    bool object_added_flag = added_object_.load();
    while (!added_object_.compare_exchange_strong(object_added_flag, true))
        ;

    return true;
}

void VulkanRenderer::reshape() {
    resized_.store(true);
}

void VulkanRenderer::onStop(rcbe::rendering::RendererStopHandlerType &&handler) {
    stop_handler_ = std::move(handler);
}

void VulkanRenderer::waitRendererReady() {
    std::unique_lock lg {waiting_mutex_};
    waiting_renderer_cv_.wait(lg, [this]() { return ready_; });
}

void VulkanRenderer::renderFrame() {
    vkWaitForFences(logical_device_, 1, std::addressof(inflight_fences_[current_frame_]), VK_TRUE, UINT64_MAX);
    [[maybe_unused]] const auto delta_time = context_->computeDeltaTime();

    uint32_t image_index;
    auto result = vkAcquireNextImageKHR(
        logical_device_,
        swap_chain_,
        UINT64_MAX,
        image_available_[current_frame_],
        VK_NULL_HANDLE,
        std::addressof(image_index));

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapchain(device_, logical_device_);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain images!");
    }

    if (images_inflight_[image_index] != VK_NULL_HANDLE) {
        vkWaitForFences(logical_device_, 1, std::addressof(images_inflight_[image_index]), VK_TRUE, UINT64_MAX);
    }
    images_inflight_[image_index] = inflight_fences_[current_frame_];

    auto persp = rdmn::render::makePerspectiveMatrix(
        NEAR_PLANE_COORDINATE,
        FAR_PLANE_COORDINATE,
        context_->getFov(),
        {static_cast<int>(swap_chain_extent_.width), static_cast<int>(swap_chain_extent_.height)});

    if (uniform_buffer_object_)
        uniform_buffer_object_->update(
            context_->getSceneTransform(),
            {},
            persp.convertUnderlyingValues<float>(),
            image_index);

    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore semaphores[] = {image_available_[current_frame_]};
    VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    std::vector<VkCommandBuffer> image_command_buffer;
    image_command_buffer.reserve(swap_chain_framebuffers_.size());
    image_command_buffer.push_back(command_buffers_[image_index]);
    submit_info.pCommandBuffers = image_command_buffer.data();

    VkSemaphore signal_semaphores[] = {render_finished_[current_frame_]};
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    vkResetFences(logical_device_, 1, std::addressof(inflight_fences_[current_frame_]));
    auto res = vkQueueSubmit(graphics_queue_, 1, std::addressof(submit_info), inflight_fences_[current_frame_]);
    if (res != VK_SUCCESS)
        throw std::runtime_error("Failed to submit draw command buffer!");

    VkPresentInfoKHR present_info {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swap_chains[] = {swap_chain_};
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = std::addressof(image_index);
    present_info.pResults = nullptr;

    res = vkQueuePresentKHR(presentation_queue_, std::addressof(present_info));

    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR || resized_ || added_object_) {
        recreateSwapchain(device_, logical_device_, added_object_.load() || resized_.load());
        resized_.store(false);
        bool added_object_value = added_object_.load();
        while (!added_object_.compare_exchange_strong(added_object_value, false))
            ;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swapchain images");
    }

    current_frame_ = (current_frame_ + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanRenderer::mainLoop() {
    while (running_) {
        renderFrame();
    }

    vkDeviceWaitIdle(logical_device_);
}

bool VulkanRenderer::createVulkanInstance(VkInstance &instance) {
    VkApplicationInfo app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = APPLICATION_NAME;
    app_info.applicationVersion = VK_MAKE_VERSION(
        rdmn::core::RDMN_ENGINE_MAJOR_VERSION,
        rdmn::core::RDMN_ENGINE_MINOR_VERSION,
        rdmn::core::RDMN_ENGINE_PATCH_VERSION);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(
        rdmn::core::RDMN_ENGINE_MAJOR_VERSION,
        rdmn::core::RDMN_ENGINE_MINOR_VERSION,
        rdmn::core::RDMN_ENGINE_PATCH_VERSION);
    app_info.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo inst_crt_info {};
    inst_crt_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_crt_info.pApplicationInfo = std::addressof(app_info);
    inst_crt_info.enabledExtensionCount = TOTAL_VK_EXTENSIONS;

    std::vector<const char *> exts;
    exts.resize(TOTAL_VK_EXTENSIONS);
    exts[0] = VK_KHR_SURFACE_EXTENSION_NAME;
    exts[1] = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
    exts[2] = "VK_KHR_get_physical_device_properties2";
    exts[3] = "VK_KHR_external_memory_capabilities";
    exts[4] = "VK_EXT_validation_features";
    exts[5] = "VK_EXT_debug_utils";

    inst_crt_info.ppEnabledExtensionNames = exts.data();

    if (config_.debug) {
        inst_crt_info.enabledLayerCount = static_cast<uint32_t>(validation_layers_.size());
        inst_crt_info.ppEnabledLayerNames = validation_layers_.data();
    } else
        inst_crt_info.enabledLayerCount = 0;

    auto res = vkCreateInstance(std::addressof(inst_crt_info), nullptr, std::addressof(instance));
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Couldn't create Vulkan instance";
        return false;
    }

    BOOST_LOG_TRIVIAL(debug) << "Vulkan instance created successfully";
    return true;
}

/// TODO: return with specification of fail if at least single is not found or not.
/// consider returning a map <extension, result>
bool VulkanRenderer::listAndCheckExtensions(std::vector<std::string> required, std::vector<OptionalExtensionRequest> optional) {
    uint32_t extensions = 0;
    auto res = vkEnumerateInstanceExtensionProperties(
        nullptr,
        std::addressof(extensions),
        nullptr);

    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Can't enumerate instance extensions properties";
        return false;
    }

    BOOST_LOG_TRIVIAL(debug) << "Total VK Extensions " << extensions;

    std::vector<VkExtensionProperties> ext_props(extensions);

    vkEnumerateInstanceExtensionProperties(nullptr, std::addressof(extensions), ext_props.data());

    BOOST_LOG_TRIVIAL(debug) << "Extensions:";

    for (const auto &e : ext_props) {
        BOOST_LOG_TRIVIAL(debug) << "\t" << e.extensionName;
    }

    if (!required.empty()) {
        for (const auto &c : required) {
            auto it = std::find_if(ext_props.begin(), ext_props.end(), [&c](const auto &entry) {
                return entry.extensionName == c;
            });

            if (it == ext_props.end()) {
                BOOST_LOG_TRIVIAL(error) << "Can't find required extension!";
                return false;
            }
        }
    }

    if (!optional.empty()) {
        for (const auto &[optional_name, fallback] : optional) {
            std::string optional_name_str(optional_name);
            const auto it = std::find_if(ext_props.begin(), ext_props.end(), [&optional_name_str](const auto &entry) {
                return entry.extensionName == optional_name_str;
            });

            if (it == ext_props.end()) {
                BOOST_LOG_TRIVIAL(debug) << "Optional extension is not found, using fallback!";
                const auto res = fallback();
                if (!res) {
                    BOOST_LOG_TRIVIAL(error) << "Optional extension fallback failed!";
                    return false;
                }
            }
        }
    }

    BOOST_LOG_TRIVIAL(debug) << "Required extension found!";
    return true;
}

bool VulkanRenderer::listAndCheckLayers(const std::vector<const char *> &to_check) {
    uint32_t layers_count = 0;
    auto res = vkEnumerateInstanceLayerProperties(std::addressof(layers_count), nullptr);
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Can't enumerate validation layers!";
        return false;
    }

    std::vector<VkLayerProperties> available_layers(layers_count);
    vkEnumerateInstanceLayerProperties(std::addressof(layers_count), available_layers.data());

    BOOST_LOG_TRIVIAL(debug) << "Total validation layers: " << layers_count;

    BOOST_LOG_TRIVIAL(debug) << "Layers:";

    for (const auto &l : available_layers) {
        BOOST_LOG_TRIVIAL(debug) << "\t" << l.layerName;
    }

    if (!to_check.empty()) {
        for (const auto &c : to_check) {
            auto it = std::find_if(available_layers.begin(), available_layers.end(), [&c](auto entry) {
                return std::string(entry.layerName) == std::string(c);
            });

            if (it == available_layers.end()) {
                BOOST_LOG_TRIVIAL(error) << "Can't find required validation layer!";
                return false;
            }
        }
    }

    return true;
}

bool VulkanRenderer::selectPhysicalDevice(VkPhysicalDevice &device) {
    XSync(context_->getDisplay(), false);
    uint32_t devices_count = 0;
    auto res = vkEnumeratePhysicalDevices(instance_, std::addressof(devices_count), nullptr);
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Can't enumerate physical devices!";
        return false;
    }

    if (devices_count == 0) {
        BOOST_LOG_TRIVIAL(debug) << "Zero physical devices found!";
        return false;
    }

    std::vector<VkPhysicalDevice> devices(devices_count);
    vkEnumeratePhysicalDevices(instance_, std::addressof(devices_count), devices.data());

    BOOST_LOG_TRIVIAL(debug) << "Searching for proper device";
    BOOST_LOG_TRIVIAL(debug) << "Total devices found " << devices.size();

    for (const auto &d : devices) {
        BOOST_LOG_TRIVIAL(debug) << "Processing device " << static_cast<uint64_t>(std::addressof(d) - devices.data());
        if (deviceSupported(d)) {
            BOOST_LOG_TRIVIAL(debug) << "Supported device found";
            device = d;
            msaa_samples_ = getMaxUsableSamplesCount();
            break;
        }
    }

    if (device == VK_NULL_HANDLE) {
        BOOST_LOG_TRIVIAL(debug) << "Couldn't find supported physical device!";
        return false;
    }

    BOOST_LOG_TRIVIAL(debug) << "Physical device selected successfully";
    return true;
}

bool VulkanRenderer::deviceSupported(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, std::addressof(properties));

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, std::addressof(features));

    auto queue_index = getQueueFamilyIndices(device);

    BOOST_LOG_TRIVIAL(debug) << "Queue index set " << std::boolalpha << queue_index.indexSet();

    auto device_ext_supported = checkDeviceExtensionSupport(device);

    bool swap_chain_fits = false;
    if (device_ext_supported) {
        auto swp_chn_sup = querySwapChainSupportDetails(device);
        if (!swp_chn_sup.obtained_successfully) {
            BOOST_LOG_TRIVIAL(debug) << "Can't obtain swap chain support details!";
            return false;
        }
        swap_chain_fits = !swp_chn_sup.formats.empty() && !swp_chn_sup.modes.empty();
    }

    BOOST_LOG_TRIVIAL(debug) << "device of right type: " << std::boolalpha << (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
    BOOST_LOG_TRIVIAL(debug) << "Has geometry shader: " << std::boolalpha << features.geometryShader;
    BOOST_LOG_TRIVIAL(debug) << "Device extension supported: " << std::boolalpha << device_ext_supported;
    BOOST_LOG_TRIVIAL(debug) << "Swap chain fits: " << std::boolalpha << swap_chain_fits;

    if (!features.shaderSampledImageArrayDynamicIndexing)
        use_global_sampler_ = true;

    return features.geometryShader && queue_index.indexSet() && device_ext_supported && swap_chain_fits && features.samplerAnisotropy;
}

VulkanRenderer::queue_family_indices VulkanRenderer::getQueueFamilyIndices(VkPhysicalDevice device) {
    queue_family_indices ret;

    uint32_t queue_families_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, std::addressof(queue_families_count), nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_families_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, std::addressof(queue_families_count), queue_families.data());

    BOOST_LOG_TRIVIAL(debug) << "Total queue families " << queue_families.size();

    int i = 0;
    for (const auto &f : queue_families) {
        BOOST_LOG_TRIVIAL(debug) << "Processing queue family index " << i;
        VkBool32 presentation_supported = vkGetPhysicalDeviceXlibPresentationSupportKHR(device, i, context_->getDisplay(), context_->visualId());

        if (presentation_supported) {
            BOOST_LOG_TRIVIAL(debug) << "Queue " << i << " supports presentation";
            ret.presentation_family = i;
        }

        if (f.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            BOOST_LOG_TRIVIAL(debug) << "Queue " << i << " supports graphics";
            ret.graphics_family = i;
        }

        if (ret.indexSet()) {
            BOOST_LOG_TRIVIAL(debug) << "Fitting queue family found!";
            break;
        }

        ++i;
    }

    return ret;
}

bool VulkanRenderer::createLogicalDevice(VkDevice &logical_device) {
    queue_family_indices index = getQueueFamilyIndices(device_);

    if (!index.indexSet()) {
        BOOST_LOG_TRIVIAL(debug) << "Queue family index is not set!";
        return false;
    }

    std::vector<VkDeviceQueueCreateInfo> create_infos;
    std::set<uint32_t> unique_queue_families = {
        index.graphics_family.value(), index.presentation_family.value()};

    auto queue_priority = 1.0f;

    for (const auto &qf : unique_queue_families) {
        VkDeviceQueueCreateInfo device_queue_crinfo {};
        device_queue_crinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_crinfo.queueFamilyIndex = qf;
        device_queue_crinfo.queueCount = 1;
        device_queue_crinfo.pQueuePriorities = std::addressof(queue_priority);
        create_infos.push_back(device_queue_crinfo);
    }

    VkPhysicalDeviceFeatures phys_dev_features {};
    phys_dev_features.samplerAnisotropy = VK_TRUE;
    phys_dev_features.sampleRateShading = VK_TRUE;

    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = static_cast<uint32_t>(create_infos.size());
    create_info.pQueueCreateInfos = create_infos.data();
    create_info.pEnabledFeatures = std::addressof(phys_dev_features);
    create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions_.size());
    create_info.ppEnabledExtensionNames = device_extensions_.data();

    if (config_.debug) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers_.size());
        create_info.ppEnabledLayerNames = validation_layers_.data();
    } else
        create_info.enabledLayerCount = 0;

    auto res = vkCreateDevice(device_, std::addressof(create_info), nullptr, std::addressof(logical_device));
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Can't create logical device";
        return false;
    }

    vkGetDeviceQueue(logical_device, index.graphics_family.value(), 0, std::addressof(graphics_queue_));
    vkGetDeviceQueue(logical_device, index.presentation_family.value(), 0, std::addressof(presentation_queue_));

    return true;
}

bool VulkanRenderer::createSurface() {
    XSync(context_->getDisplay(), false);
    VkXlibSurfaceCreateInfoKHR surf_create_info {};
    surf_create_info.dpy = context_->getDisplay();
    surf_create_info.window = context_->getDrawable();
    surf_create_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;

    auto result = vkCreateXlibSurfaceKHR(instance_, std::addressof(surf_create_info), nullptr, std::addressof(surface_));

    if (result != VK_SUCCESS)
        BOOST_LOG_TRIVIAL(debug) << "Can't create XLib surface!";

    BOOST_LOG_TRIVIAL(debug) << "Surface created successfully!";

    return result == VK_SUCCESS;
}

bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t device_extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, std::addressof(device_extension_count), nullptr);

    std::vector<VkExtensionProperties> available_extensions(device_extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, std::addressof(device_extension_count), available_extensions.data());

    std::set<std::string> required_extensions(device_extensions_.begin(), device_extensions_.end());

    BOOST_LOG_TRIVIAL(debug) << "total required extensions: " << required_extensions.size();

    for (const auto &re : required_extensions) {
        BOOST_LOG_TRIVIAL(debug) << "\t" << re;
    }

    BOOST_LOG_TRIVIAL(debug) << "Available physical device extensions amount: " << available_extensions.size();

    BOOST_LOG_TRIVIAL(debug) << "Extensions: ";
    for (const auto &e : available_extensions) {
        BOOST_LOG_TRIVIAL(debug) << "\t" << e.extensionName;
        required_extensions.erase(e.extensionName);
    }

    BOOST_LOG_TRIVIAL(debug) << "Extension found: " << std::boolalpha << required_extensions.empty();

    return required_extensions.empty();
}

VulkanRenderer::swap_chain_support_details VulkanRenderer::querySwapChainSupportDetails(VkPhysicalDevice device) {
    swap_chain_support_details ret;

    auto res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, std::addressof(ret.capabilities));
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Can't get device surface capabilities";
        ret.obtained_successfully = false;
        return ret;
    }

    uint32_t format_count;
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, std::addressof(format_count), nullptr);
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Can't get device surface formats";
        ret.obtained_successfully = false;
        return ret;
    }

    if (format_count != 0) {
        ret.formats.resize(format_count);
        res = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, std::addressof(format_count), ret.formats.data());
        if (res != VK_SUCCESS) {
            BOOST_LOG_TRIVIAL(debug) << "Can't get device surface formats";
            ret.obtained_successfully = false;
            return ret;
        }
    }

    uint32_t pres_mode_count;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, std::addressof(pres_mode_count), nullptr);
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Can't get device surface present modes";
        ret.obtained_successfully = false;
        return ret;
    }

    if (pres_mode_count != 0) {
        ret.modes.resize(pres_mode_count);
        res = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, std::addressof(pres_mode_count), ret.modes.data());
        if (res != VK_SUCCESS) {
            BOOST_LOG_TRIVIAL(debug) << "Can't get device surface present modes";
            ret.obtained_successfully = false;
            return ret;
        }
    }

    BOOST_LOG_TRIVIAL(debug) << "Returning swap chain details";

    ret.obtained_successfully = true;
    return ret;
}

VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &available_formats) {
    for (const auto &af : available_formats) {
        if (af.format == VK_FORMAT_R8G8B8A8_SRGB && af.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
            return af;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR VulkanRenderer::choosePresentMode(const std::vector<VkPresentModeKHR> &available_pres_modes) {
    for (const auto &apm : available_pres_modes) {
        if (apm == VK_PRESENT_MODE_MAILBOX_KHR) {
            return apm;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actual;

        if (resized_) {
            actual = {
                static_cast<uint32_t>(context_->getWindowDimensions().width),
                static_cast<uint32_t>(context_->getWindowDimensions().height)};
        } else {
            actual = {
                static_cast<uint32_t>(config_.initial_dimensions.width),
                static_cast<uint32_t>(config_.initial_dimensions.height)};

            const auto &supp_w = capabilities.currentExtent.width;
            const auto &supp_h = capabilities.currentExtent.height;

            actual.width = std::max(supp_w, std::min(supp_w, actual.width));
            actual.height = std::max(supp_h, std::min(supp_h, actual.height));
        }

        return actual;
    }
}

bool VulkanRenderer::createSwapChain(VkPhysicalDevice device) {
    swap_chain_support_details swp_chn_sup = querySwapChainSupportDetails(device);
    VkSurfaceFormatKHR surf_format = chooseSwapSurfaceFormat(swp_chn_sup.formats);
    VkPresentModeKHR pres_mode = choosePresentMode(swp_chn_sup.modes);
    swap_chain_extent_ = chooseSwapExtent(swp_chn_sup.capabilities);
    swap_chain_format_ = surf_format.format;

    auto images_count = swp_chn_sup.capabilities.minImageCount + 1;// TODO: make increment configurable

    if (swp_chn_sup.capabilities.maxImageCount > 0 && images_count > swp_chn_sup.capabilities.maxImageCount) {
        images_count = swp_chn_sup.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface_;
    create_info.minImageCount = images_count;
    create_info.imageFormat = swap_chain_format_;
    create_info.imageColorSpace = surf_format.colorSpace;
    create_info.imageExtent = swap_chain_extent_;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (resized_) {
        create_info.oldSwapchain = swap_chain_;
    }

    auto indicies = getQueueFamilyIndices(device);

    if (!indicies.indexSet()) {
        BOOST_LOG_TRIVIAL(debug) << "Queue family indices not found!";
        return false;
    }

    uint32_t queue_family_indices[] = {indicies.graphics_family.value(), indicies.presentation_family.value()};
    if (indicies.graphics_family.value() != indicies.presentation_family.value()) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = swp_chn_sup.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = pres_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    auto res = vkCreateSwapchainKHR(logical_device_, std::addressof(create_info), nullptr, std::addressof(swap_chain_));
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(debug) << "Can't create swapchain";
        return false;
    }

    vkGetSwapchainImagesKHR(logical_device_, swap_chain_, std::addressof(images_count), nullptr);
    swap_chain_images_.resize(images_count);
    vkGetSwapchainImagesKHR(logical_device_, swap_chain_, std::addressof(images_count), swap_chain_images_.data());

    return (res == VK_SUCCESS);
}

VulkanRenderer::image_view_creation_result VulkanRenderer::createSwapChainViews() {
    swap_chain_views_.resize(swap_chain_images_.size());

    size_t i = 0;
    for (const auto &swi : swap_chain_images_) {
        auto opt_res = createImageView(logical_device_, swi, swap_chain_format_, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        if (!opt_res) {
            return {.success = false, .failed_index = i};
        }
        swap_chain_views_[i] = *opt_res;
        ++i;
    }

    return {.success = true, .failed_index = 0};
}

bool VulkanRenderer::createGraphicsPipeline(VkDevice logical_device) {
    pipelines_.reserve(material_cache_.size());
    size_t counter = 0;
    for (auto &m : material_cache_) {
        auto &material = m->as<rcbe::rendering::Material>();
        try {
            pipelines_.insert(
                {m,
                 std::make_shared<VulkanGraphicsPipeline>(material,
                                                          logical_device,
                                                          swap_chain_extent_,
                                                          msaa_samples_,
                                                          descriptor_set_layout_,
                                                          render_pass_,
                                                          sizeof(MeshPushConstants))});
            ++counter;
        } catch (const std::exception &e) {
            BOOST_LOG_TRIVIAL(error) << e.what();
            return false;
        }
    }

    return true;
}

bool VulkanRenderer::createRenderPass(VkDevice logical_device) {
    VkAttachmentDescription color_attachment {};
    color_attachment.format = swap_chain_format_;
    color_attachment.samples = msaa_samples_;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depth_attachment {};
    auto opt_dep_form = findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        device_,
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    if (!opt_dep_form) {
        BOOST_LOG_TRIVIAL(error) << "Can't find depth format!";
        return false;
    }

    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(device_, VK_FORMAT_R8G8B8A8_SRGB, std::addressof(props));

    if (!(props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw std::runtime_error("Format doesn't fit mip maps!");
    }

    depth_attachment.format = *opt_dep_form;
    depth_attachment.samples = msaa_samples_;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription color_attachment_resolve {};
    color_attachment_resolve.format = swap_chain_format_;
    color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attach_ref {};
    color_attach_ref.attachment = 0;
    color_attach_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attach_ref {};
    depth_attach_ref.attachment = 1;
    depth_attach_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference color_attachment_resolve_ref {};
    color_attachment_resolve_ref.attachment = 2;
    color_attachment_resolve_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass_description {};
    subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_description.colorAttachmentCount = 1;
    subpass_description.pColorAttachments = std::addressof(color_attach_ref);
    subpass_description.pDepthStencilAttachment = std::addressof(depth_attach_ref);
    subpass_description.pResolveAttachments = std::addressof(color_attachment_resolve_ref);

    std::array<VkAttachmentDescription, 3> attachments = {
        color_attachment, depth_attachment, color_attachment_resolve};

    VkSubpassDependency subpass_dep {};
    subpass_dep.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpass_dep.dstSubpass = 0;
    subpass_dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dep.srcAccessMask = 0;
    subpass_dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
    create_info.pAttachments = attachments.data();
    create_info.subpassCount = 1;
    create_info.pSubpasses = std::addressof(subpass_description);
    create_info.dependencyCount = 1;
    create_info.pDependencies = std::addressof(subpass_dep);

    auto res = vkCreateRenderPass(logical_device, std::addressof(create_info), nullptr, std::addressof(render_pass_));
    if (res != VK_SUCCESS)
        return false;

    return true;
}

bool VulkanRenderer::createFramebuffers(VkDevice logcal_device) {
    swap_chain_framebuffers_.resize(swap_chain_views_.size());

    for (size_t i = 0; i < swap_chain_views_.size(); ++i) {
        std::array<VkImageView, 3> attach = {
            color_image_view_,
            depth_image_view_,
            swap_chain_views_[i]};

        VkFramebufferCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        create_info.renderPass = render_pass_;
        create_info.attachmentCount = static_cast<uint32_t>(attach.size());
        create_info.pAttachments = attach.data();
        create_info.width = swap_chain_extent_.width;
        create_info.height = swap_chain_extent_.height;
        create_info.layers = 1;

        auto res = vkCreateFramebuffer(logical_device_, std::addressof(create_info), nullptr, std::addressof(swap_chain_framebuffers_[i]));
        if (res != VK_SUCCESS) {
            for (size_t j = i - 1; j >= 0; --j) {
                vkDestroyFramebuffer(logcal_device, swap_chain_framebuffers_[j], nullptr);
            }

            return false;
        }
    }

    return true;
}

bool VulkanRenderer::createCommandPool(VkDevice logical_device) {
    auto queue_family_indices = getQueueFamilyIndices(device_);

    if (!queue_family_indices.indexSet())
        return false;

    VkCommandPoolCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.queueFamilyIndex = queue_family_indices.graphics_family.value();
    create_info.flags = 0;

    auto res = vkCreateCommandPool(logical_device, std::addressof(create_info), nullptr, std::addressof(command_pool_));
    if (res != VK_SUCCESS) {
        return false;
    }

    return true;
}

bool VulkanRenderer::createCommandBuffers(VkDevice logical_device) {
    size_t counter = 0;

    command_buffers_.resize(swap_chain_framebuffers_.size());

    VkCommandBufferAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool_;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = static_cast<uint32_t>(swap_chain_framebuffers_.size());

    auto res = vkAllocateCommandBuffers(
        logical_device,
        std::addressof(alloc_info),
        command_buffers_.data());

    if (res != VK_SUCCESS)
        return false;

    for (size_t i = 0; i < swap_chain_framebuffers_.size(); ++i) {
        VkCommandBufferBeginInfo binfo {};
        binfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        binfo.flags = 0;
        binfo.pInheritanceInfo = nullptr;

        auto result = vkBeginCommandBuffer(command_buffers_[i], std::addressof(binfo));
        if (result != VK_SUCCESS) {
            for (size_t j = i - 1; j >= 0; --j) {
                vkEndCommandBuffer(command_buffers_[j]);
            }

            return false;
        }

        VkRenderPassBeginInfo rpass_binfo {};
        rpass_binfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpass_binfo.renderPass = render_pass_;
        rpass_binfo.framebuffer = swap_chain_framebuffers_[i];
        rpass_binfo.renderArea.offset = {0, 0};
        rpass_binfo.renderArea.extent = swap_chain_extent_;

        std::array<VkClearValue, 2> clear_values {};

        clear_values[0].color = {
            static_cast<float>(config_.clear_color.r()),
            static_cast<float>(config_.clear_color.g()),
            static_cast<float>(config_.clear_color.b()),
            static_cast<float>(config_.clear_color.a())};
        clear_values[1].depthStencil = {1.0f, 0};
        rpass_binfo.clearValueCount = static_cast<uint32_t>(clear_values.size());
        rpass_binfo.pClearValues = clear_values.data();

        vkCmdBeginRenderPass(
            command_buffers_[i],
            std::addressof(rpass_binfo),
            VK_SUBPASS_CONTENTS_INLINE);

        size_t prev_verts_count = 0;
        size_t prev_last_index = 0;

        for (auto it = pipelines_.begin(); it != pipelines_.end(); ++it) {
            const auto &[material_object, pipeline] = *it;

            vkCmdBindPipeline(
                command_buffers_[i],
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipeline->getPipeline());

            /// !IMPORTANT bind buffer here
            if (buffer_object_)
                buffer_object_->bind(
                    command_buffers_[i],
                    0,
                    0);
            else
                BOOST_LOG_TRIVIAL(error) << "No VBO";

            if (uniform_buffer_object_)
                uniform_buffer_object_->bind(
                    command_buffers_[i],
                    pipeline->getLayout(),
                    i);
            else
                BOOST_LOG_TRIVIAL(error) << "No UBO";


            {
                const auto &o = objects_[material_object][0];
                const auto &mesh = o.getComponent<rcbe::geometry::Mesh>()->as<rcbe::geometry::Mesh>();
                const auto &material = o.getComponent<rcbe::rendering::Material>()->as<rcbe::rendering::Material>();

                MeshPushConstants mpc {};
                mpc.model_matrix = mesh.getTransform().matrix().transposed().convertUnderlyingValues<float>();

                mpc.sampler_index = object_to_sampler_index_[o.hash()].at(0);
                if (object_to_sampler_index_[o.hash()].size() > 1) {
                    mpc.sampler_secondary_index = object_to_sampler_index_[o.hash()].at(1);
                }

                vkCmdPushConstants(command_buffers_[i],
                                   pipeline->getLayout(),
                                   VK_SHADER_STAGE_VERTEX_BIT,
                                   0,
                                   sizeof(MeshPushConstants),
                                   std::addressof(mpc));

                vkCmdDrawIndexed(command_buffers_[i],
                                 mesh.facetsSize() * 3,
                                 1, prev_last_index,
                                 prev_verts_count, 0);

                prev_verts_count += mesh.verticesSize();
                prev_last_index += mesh.facetsSize() * 3;

                if (objects_[material_object].size() == 1) {
                    const auto transform =
                        objects_[material_object][0].getComponent<rcbe::geometry::Mesh>()->as<rcbe::geometry::Mesh>().getTransform();
                }
            }

            for (size_t j = 1; j < objects_[material_object].size(); ++j) {
                const auto &prev_mesh =
                    objects_[material_object][j - 1].getComponent<rcbe::geometry::Mesh>()->as<rcbe::geometry::Mesh>();
                const auto &mesh =
                    objects_[material_object][j].getComponent<rcbe::geometry::Mesh>()->as<rcbe::geometry::Mesh>();

                MeshPushConstants mpc {};
                mpc.model_matrix = mesh.getTransform().matrix().transposed().convertUnderlyingValues<float>();
                mpc.sampler_index = object_to_sampler_index_[objects_[material_object][j].hash()].at(0);
                if (object_to_sampler_index_[objects_[material_object][j].hash()].size() > 1) {
                    mpc.sampler_secondary_index = object_to_sampler_index_[objects_[material_object][j].hash()].at(1);
                }

                vkCmdPushConstants(command_buffers_[i],
                                   pipeline->getLayout(),
                                   VK_SHADER_STAGE_VERTEX_BIT,
                                   0,
                                   sizeof(MeshPushConstants),
                                   std::addressof(mpc));

                vkCmdDrawIndexed(command_buffers_[i],
                                 mesh.facetsSize() * 3,
                                 1, prev_last_index,
                                 prev_verts_count, 0);

                prev_verts_count += mesh.verticesSize();
                prev_last_index += mesh.facetsSize() * 3;
            }
        }
    }

    for (size_t i = 0; i < swap_chain_framebuffers_.size(); ++i) {
        vkCmdEndRenderPass(command_buffers_[i]);
        auto result = vkEndCommandBuffer(command_buffers_[i]);
        if (result != VK_SUCCESS) {
            for (size_t j = i - 1; j >= 0; --j) {
                vkEndCommandBuffer(command_buffers_[j]);
            }

            return false;
        }
    }

    return true;
}

bool VulkanRenderer::createSyncObjects(VkDevice logical_device) {
    image_available_.resize(MAX_FRAMES_IN_FLIGHT);
    render_finished_.resize(MAX_FRAMES_IN_FLIGHT);
    inflight_fences_.resize(MAX_FRAMES_IN_FLIGHT);
    images_inflight_.resize(swap_chain_images_.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo screate_info {};
    screate_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_create {};
    fence_create.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        if (vkCreateSemaphore(logical_device, std::addressof(screate_info), nullptr, std::addressof(image_available_[i])) != VK_SUCCESS || vkCreateSemaphore(logical_device, std::addressof(screate_info), nullptr, std::addressof(render_finished_[i])) != VK_SUCCESS || vkCreateFence(logical_device, std::addressof(fence_create), nullptr, std::addressof(inflight_fences_[i])) != VK_SUCCESS) {
            BOOST_LOG_TRIVIAL(error) << "Can't create semaphores!";
            return false;
        }
    }

    return true;
}

bool VulkanRenderer::recreateSwapchain(VkPhysicalDevice device, VkDevice logical_device, bool handle_rendering_objects) {
    while (context_->getWindowDimensions().width == 0 || context_->getWindowDimensions().height == 0) {
        BOOST_LOG_TRIVIAL(debug) << "Window is minimized!";
    }

    vkDeviceWaitIdle(logical_device);

    cleanupSwapchain(false);

    {
        auto res = createSwapChain(device);
        if (!res) {
            BOOST_LOG_TRIVIAL(error) << "Can't recreate swapchain!";
            return false;
        }
    }

    {
        auto res = createSwapChainViews();
        if (!res.success) {
            BOOST_LOG_TRIVIAL(error) << "Can't create swapchain views!";
            return false;
        }
    }

    {
        auto res = createRenderPass(logical_device);
        if (!res) {
            BOOST_LOG_TRIVIAL(error) << "Can't recreate render pass!";
            return false;
        }
    }

    preexistant_objects_load_result preexistent_objects_data {};
    {
        preexistent_objects_data = loadPreexistentObjects(handle_rendering_objects);
        if (!preexistent_objects_data.success)
            throw std::runtime_error("Can't load preexistent objects into scene!");
    }

    {
        auto res = createGraphicsPipeline(logical_device);
        if (!res) {
            BOOST_LOG_TRIVIAL(error) << "Can't recreate graphics pipeline!";
            return false;
        }
    }

    {
        auto res = createColorResources(logical_device);
        if (!res) {
            BOOST_LOG_TRIVIAL(error) << "Can't recreate color buffer!";
            return false;
        }
    }

    {
        auto res = createDepthResources(logical_device);
        if (!res) {
            BOOST_LOG_TRIVIAL(error) << "Can't recreate depth buffer!";
            return false;
        }
    }

    {
        auto res = createFramebuffers(logical_device);
        if (!res) {
            BOOST_LOG_TRIVIAL(error) << "Can't recreate framebuffers!";
            return false;
        }
    }

    if (!handle_rendering_objects) {
        BOOST_LOG_TRIVIAL(info) << "Here?";
        auto res = initPreexistentTextures(preexistent_objects_data);
        if (!res) {
            BOOST_LOG_TRIVIAL(error) << "Can't init preexistent textures!";
            return false;
        }
    }

    texture_to_index_.clear();
    auto [s, v] = repopulateSamplersAndViews(texture_to_index_, material_cache_, use_global_sampler_);

    repopulateTextureIndices();

    if (handle_rendering_objects) {
        buffer_object_.reset();
        buffer_object_ = std::make_shared<VulkanVertexBufferObject>(
            preexistent_objects_data.meshes,
            logical_device_,
            device_,
            presentation_queue_,
            command_pool_);
    }

    if (use_global_sampler_) {
        uniform_buffer_object_ = std::make_shared<VulkanUniformBufferObject>(
            logical_device_,
            device_,
            descriptor_set_layout_,
            rasterizer_tex_global_sampler_,
            std::move(v),
            TOTAL_SHADER_TEXTURES,
            swap_chain_images_.size());
    } else {
        uniform_buffer_object_ = std::make_shared<VulkanUniformBufferObject>(
            logical_device_,
            device_,
            descriptor_set_layout_,
            std::move(v),
            std::move(s),
            swap_chain_images_.size());
    }

    {
        auto res = createCommandBuffers(logical_device);
        if (!res) {
            BOOST_LOG_TRIVIAL(error) << "Can't recreate command buffers!";
            return false;
        }
    }

    return true;
}

bool VulkanRenderer::createDescriptorSetLayout(VkDevice logical_device) {
    VkDescriptorSetLayoutBinding ubo_layout_binding {};
    ubo_layout_binding.binding = 0;
    ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    ubo_layout_binding.descriptorCount = 1;
    ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    ubo_layout_binding.pImmutableSamplers = nullptr;

    BOOST_LOG_TRIVIAL(trace) << std::boolalpha << " use global sampler " << use_global_sampler_;

    VkDescriptorSetLayoutBinding sampler_layout_binding {};
    sampler_layout_binding.binding = 1;
    sampler_layout_binding.descriptorType = (use_global_sampler_)
                                                ? VK_DESCRIPTOR_TYPE_SAMPLER
                                                : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sampler_layout_binding.descriptorCount = 1;
    sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    sampler_layout_binding.pImmutableSamplers = nullptr;

    std::vector<VkDescriptorSetLayoutBinding> bindings = {
        ubo_layout_binding,
        sampler_layout_binding};

    if (use_global_sampler_) {
        VkDescriptorSetLayoutBinding textures_layout_binding {};
        textures_layout_binding.binding = 2;
        textures_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        textures_layout_binding.descriptorCount = TOTAL_SHADER_TEXTURES * swap_chain_views_.size();
        textures_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        textures_layout_binding.pImmutableSamplers = nullptr;
        bindings.push_back(textures_layout_binding);
    }

    std::vector<VkDescriptorBindingFlags> bind_flags(2, 0);
    if (use_global_sampler_) {
        bind_flags.push_back(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT);
    }

    VkDescriptorSetLayoutBindingFlagsCreateInfo bflags_info {};
    bflags_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    bflags_info.bindingCount = static_cast<uint32_t>(bindings.size());
    bflags_info.pBindingFlags = bind_flags.data();

    VkDescriptorSetLayoutCreateInfo layout_create_info {};
    layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_create_info.bindingCount = static_cast<uint32_t>(bindings.size());
    layout_create_info.pBindings = bindings.data();
    layout_create_info.pNext = std::addressof(bflags_info);

    auto res = vkCreateDescriptorSetLayout(
        logical_device,
        std::addressof(layout_create_info),
        nullptr,
        std::addressof(descriptor_set_layout_));
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(error) << "Can't create descriptor set layout";
        return false;
    }

    return true;
}

bool VulkanRenderer::createDepthResources(VkDevice logical_device) {
    auto opt_format = findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        device_,
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    if (!opt_format) {
        BOOST_LOG_TRIVIAL(error) << "No suitable format found!";
        return false;
    }

    auto format = *opt_format;
    createImage(
        logical_device,
        device_,
        {static_cast<int>(swap_chain_extent_.width), static_cast<int>(swap_chain_extent_.height)},
        format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        1,
        msaa_samples_,
        depth_image_,
        depth_image_memory_);
    auto dep_img_view_opt = createImageView(logical_device, depth_image_, format, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    if (!dep_img_view_opt) {
        BOOST_LOG_TRIVIAL(error) << "Can't create depth image view!";
        return false;
    }

    depth_image_view_ = *dep_img_view_opt;

    return true;
}

std::optional<VkFormat> VulkanRenderer::findSupportedFormat(
    const std::vector<VkFormat> &candidates,
    VkPhysicalDevice physical_device,
    VkImageTiling tiling,
    VkFormatFeatureFlags features) {
    for (const auto &format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physical_device, format, std::addressof(props));

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    return std::nullopt;
}

bool VulkanRenderer::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkSampleCountFlagBits VulkanRenderer::getMaxUsableSamplesCount() {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(device_, std::addressof(props));

    VkSampleCountFlags counts = props.limits.framebufferColorSampleCounts & props.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT)
        return VK_SAMPLE_COUNT_64_BIT;

    if (counts & VK_SAMPLE_COUNT_32_BIT)
        return VK_SAMPLE_COUNT_32_BIT;

    if (counts & VK_SAMPLE_COUNT_16_BIT)
        return VK_SAMPLE_COUNT_16_BIT;

    if (counts & VK_SAMPLE_COUNT_8_BIT)
        return VK_SAMPLE_COUNT_8_BIT;

    if (counts & VK_SAMPLE_COUNT_4_BIT)
        return VK_SAMPLE_COUNT_4_BIT;

    if (counts & VK_SAMPLE_COUNT_2_BIT)
        return VK_SAMPLE_COUNT_2_BIT;

    return VK_SAMPLE_COUNT_1_BIT;
}

bool VulkanRenderer::createColorResources(VkDevice logical_device) {
    VkFormat color_format = swap_chain_format_;
    if (!createImage(
            logical_device,
            device_,
            {static_cast<int>(swap_chain_extent_.width), static_cast<int>(swap_chain_extent_.height)},
            color_format,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            1,
            msaa_samples_,
            color_image_,
            color_image_memory_)) {
        BOOST_LOG_TRIVIAL(error) << "Can't create color image!";
        return false;
    }

    auto opt_img_view = createImageView(
        logical_device,
        color_image_,
        color_format,
        VK_IMAGE_ASPECT_COLOR_BIT,
        1);

    if (!opt_img_view) {
        BOOST_LOG_TRIVIAL(error) << "Can't create image view!";
        return false;
    }

    color_image_view_ = *opt_img_view;

    return true;
}

VulkanRenderer::preexistant_objects_load_result VulkanRenderer::loadPreexistentObjects(bool secondary_invokation) {
    preexistant_objects_load_result ret {};
    auto &meshes = ret.meshes;
    auto &rasterizer_tex_samplers = ret.rasterizer_tex_samplers;
    auto &rasterizer_tex_image_views = ret.rasterizer_tex_image_views;
    try {
        const auto calculate_mip_levels_wrapper = [this]() -> size_t {
            if (!material_cache_.empty()) {
                auto it = material_cache_.begin();
                const auto &mat = (*it)->getComponent<rcbe::rendering::Material>()->as<rcbe::rendering::Material>();
                if (!mat.getTextures().empty()) {
                    const auto &[vt, rt] = *(mat.getTextures().begin());
                    return calculateMipLevels(vt->getWidth(), vt->getHeight());
                } else {
                    return calculateMipLevels(config_.initial_dimensions.width, config_.initial_dimensions.height);
                }
            } else {
                return calculateMipLevels(config_.initial_dimensions.width, config_.initial_dimensions.height);
            }
        };
        if (!secondary_invokation) {
            auto edge_object = loadCorner();
            meshes.reserve(1);
            object_to_sampler_index_.reserve(1);

            meshes.push_back(edge_object.getComponent<rcbe::geometry::Mesh>()->as<rcbe::geometry::Mesh>());

            if (use_global_sampler_) {
                size_t mip_levels = calculate_mip_levels_wrapper();

                auto res = createTextureSampler(
                    logical_device_,
                    rasterizer_tex_global_sampler_,
                    mip_levels);
                if (!res) {
                    throw std::runtime_error("Can't create global image sampler");
                }
            }

            std::unordered_set<rcbe::visual::TexturePtr> tex_cache;

            {
                const auto [it, res] = material_cache_.insert(edge_object.getComponent<rcbe::rendering::Material>());
                if (objects_[*it].empty())
                    objects_[*it].reserve(1);
                objects_[*it].push_back(std::move(edge_object));
            }

            for (auto &[pipeline, objects_collection] : objects_) {
                for (size_t i = 0; i < objects_collection.size(); ++i) {
                    auto &c = objects_collection[i];
                    const auto &tex_component = c.getComponent<rdmn::render::RasterizerTexture>();
                    if (!tex_component)
                        continue;
                    auto &rt = tex_component->as<rdmn::render::RasterizerTexture>();
                    const auto &vtex_ptr = rt.getVisualTexturePtr();

                    const auto it = tex_cache.find(vtex_ptr);
                    if (it == tex_cache.end()) {
                        const auto [vtex_it, res] = tex_cache.insert(vtex_ptr);
                    }
                }
            }

            for (const auto &o : material_cache_) {
                const auto &mat = o->as<rcbe::rendering::Material>();
                for (const auto &[v, t] : mat.getTextures()) {
                    if (!use_global_sampler_)
                        rasterizer_tex_samplers.push_back(t->getImageSampler());
                    rasterizer_tex_image_views.push_back(t->getImageView());
                }
            }

            for (auto &[pipeline_ptr, objects_collections] : objects_) {
                for (size_t i = 0; i < objects_collections.size(); ++i) {
                    {
                        if (objects_collections[i].hasComponent<RasterizerTexture>()) {
                            const auto &rt_comp = objects_collections[i].getComponent<rdmn::render::RasterizerTexture>();
                            const auto &rt = rt_comp->as<rdmn::render::RasterizerTexture>();
                            const auto &vtex = rt.getVisualTexturePtr();
                            const auto it = tex_cache.find(rt.getVisualTexturePtr());
                        }
                    }
                }
            }
        } else {
            BOOST_LOG_TRIVIAL(debug) << "Secondary objects invokation";
            if (use_global_sampler_) {
                auto res = createTextureSampler(
                    logical_device_,
                    rasterizer_tex_global_sampler_,
                    calculate_mip_levels_wrapper());
                if (!res) {
                    throw std::runtime_error("Can't create global image sampler");
                }
            }

            std::unordered_set<rcbe::visual::TexturePtr> tex_cache;

            auto reserve_val = std::accumulate(objects_.begin(), objects_.end(), 0, [](auto l, const auto &r) {
                return l + r.second.size();
            });
            meshes.reserve(reserve_val);

            for (const auto &[material_object_ptr, object_collection] : objects_) {
                const auto &m = material_object_ptr->as<rcbe::rendering::Material>();

                if (!m.getVertex()) {
                    BOOST_LOG_TRIVIAL(warning) << "Vertex is not present";
                }

                if (!m.getFragment()) {
                    BOOST_LOG_TRIVIAL(warning) << "Fragment is not present";
                }

                material_cache_.insert(material_object_ptr);

                std::transform(object_collection.begin(), object_collection.end(), std::back_inserter(meshes), [](const auto &entry) {
                    return entry.template getComponent<rcbe::geometry::Mesh>()->template as<rcbe::geometry::Mesh>();
                });
            }

            for (const auto &o : material_cache_) {
                const auto &mat = o->as<rcbe::rendering::Material>();
                for (const auto &[v, t] : mat.getTextures()) {
                    if (!use_global_sampler_)
                        rasterizer_tex_samplers.push_back(t->getImageSampler());
                    rasterizer_tex_image_views.push_back(t->getImageView());
                }
            }
        }
    } catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "Can't parse scene! " << e.what();
        throw;
    }

    ret.success = true;
    return ret;
}

bool VulkanRenderer::initPreexistentTextures(preexistant_objects_load_result &objects) {
    for (const auto &m : material_cache_) {
        const auto &mat = m->as<rcbe::rendering::Material>();
        if (!initMaterialTextures(mat))
            return false;
    }

    return true;
}

void VulkanRenderer::repopulateTextureIndices() {
    object_to_sampler_index_.clear();
    for (const auto &[material_ptr, object_collection] : objects_) {
        for (const auto &object : object_collection) {
            if (!object.hasComponent<rdmn::render::RasterizerTexture>()) {
                if (!object.hasComponent<rcbe::visual::VisualTextureSet>()) {
                    object_to_sampler_index_[object.hash()].push_back(0);
                    continue;
                } else {
                    const auto &tex_set = object.getComponent<rcbe::visual::VisualTextureSet>()->as<rcbe::visual::VisualTextureSet>();
                    for (const auto &vtex : tex_set.grouped_textures) {
                        auto it = texture_to_index_.find(vtex);
                        if (it != texture_to_index_.end()) {
                            object_to_sampler_index_[object.hash()].push_back(it->second);
                        } else {
                            object_to_sampler_index_[object.hash()].push_back(0);
                        }
                    }
                }
            } else {
                const auto &vtex =
                    object.getComponent<rdmn::render::RasterizerTexture>()->as<rdmn::render::RasterizerTexture>().getVisualTexturePtr();
                auto it = texture_to_index_.find(vtex);
                if (it != texture_to_index_.end()) {
                    object_to_sampler_index_[object.hash()].push_back(it->second);
                } else {
                    object_to_sampler_index_[object.hash()].push_back(0);
                }
            }
        }
    }
}

bool VulkanRenderer::initMaterialTextures(const rcbe::rendering::Material &mat) {
    for (const auto &[_, inserted_tex] : mat.getTextures()) {
        if (use_global_sampler_) {
            if (!inserted_tex->init(
                    logical_device_, device_, command_pool_, graphics_queue_, rasterizer_tex_global_sampler_)) {
                BOOST_LOG_TRIVIAL(error) << "Can't init texture!";
                return false;
            } else {
                BOOST_LOG_TRIVIAL(trace) << "Successfully inited texture";
            }
        } else {
            if (!inserted_tex->init(logical_device_, device_, command_pool_, graphics_queue_)) {
                BOOST_LOG_TRIVIAL(error) << "Can't init texture!";
                return false;
            } else {
                BOOST_LOG_TRIVIAL(trace) << "Successfully inited texture";
            }
        }
    }
    return true;
}

void VulkanRenderer::handleRenderedObject(
    rcbe::core::CoreObject &renderer_object,
    std::unordered_map<std::shared_ptr<rcbe::core::CoreObject>, std::vector<rcbe::core::CoreObject>> &objects,
    std::unordered_set<std::shared_ptr<rcbe::core::CoreObject>> &material_cache) {
    const auto &mat_ptr = renderer_object.getComponent<rcbe::rendering::Material>();

    auto it = material_cache.find(mat_ptr);
    if (it == material_cache.end()) {
        auto [inserted_it, res] = material_cache.insert(mat_ptr);
        it = inserted_it;
        const auto &m = (*it)->as<rcbe::rendering::Material>();
        if (!initMaterialTextures(m)) {
            BOOST_LOG_TRIVIAL(fatal) << "Can't init texture of added object " << renderer_object.name();
        }
    }
    if (objects[*it].empty())
        objects[*it].reserve(1);

    objects[*it].push_back(std::move(renderer_object));
}

}// namespace rdmn::render