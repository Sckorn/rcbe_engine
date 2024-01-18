#include <mutex>

#include <boost/log/trivial.hpp>

#include <rcbe-engine/datamodel/rendering/VulkanBufferObject.hpp>
#include <rcbe-engine/datamodel/rendering/buffer_object_helpers.hpp>


namespace {

template <typename UnderlyingIndexType>
struct VulkanIndexType;

template <>
struct VulkanIndexType<uint16_t> {
    static constexpr auto value = VK_INDEX_TYPE_UINT16;
};

template <>
struct VulkanIndexType<uint32_t> {
    static constexpr auto value = VK_INDEX_TYPE_UINT32;
};

inline constexpr size_t VERTEX_ARRAY_OBJECT_ID = 0;
inline constexpr size_t NORMAL_ARRAY_OBJECT_ID = 1;
inline constexpr size_t COLOR_ARRAY_OBJECT_ID = 2;
inline constexpr size_t TEXCOORD_ARRAY_OBJECT_ID = 3;

bool createVertexBuffer(VkDevice &logical_device,
                        VkPhysicalDevice &device,
                        VkBuffer &buff,
                        VkDeviceMemory &memory,
                        VkQueue &presentation_queue,
                        VkCommandPool &command_pool,
                        const rdmn::render::VulkanVertexBufferObject::StorageType &source) {
    auto buffer_size = sizeof(rdmn::render::VulkanVertexBufferObject::StorageType::value_type) * source.size();

    auto res = rdmn::render::createGeneralBuffer(
        buffer_size,
        logical_device,
        device,
        buff,
        memory,
        presentation_queue,
        command_pool,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        source);

    if (!res) {
        BOOST_LOG_TRIVIAL(error) << "Can't create buffer!";
        return false;
    }

    return true;
}

bool createIndexBuffer(VkDevice &logical_device,
                       VkPhysicalDevice &device,
                       VkBuffer &buff,
                       VkDeviceMemory &memory,
                       VkQueue &presentation_queue,
                       VkCommandPool &command_pool,
                       const rdmn::render::VulkanVertexBufferObject::IndicesStorageType &source) {
    auto buffer_size = sizeof(rdmn::render::VulkanVertexBufferObject::IndicesStorageType::value_type) * source.size();

    auto res = rdmn::render::createGeneralBuffer(
        buffer_size,
        logical_device,
        device,
        buff,
        memory,
        presentation_queue,
        command_pool,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        source);

    if (!res) {
        BOOST_LOG_TRIVIAL(error) << "Can't create buffer!";
        return false;
    }

    return true;
}

}// namespace

namespace rdmn::render {
VulkanVertexBufferObject::VulkanVertexBufferObject(
    const std::vector<rcbe::geometry::Mesh> &meshes,
    VkDevice &logical_device,
    VkPhysicalDevice &device,
    VkQueue &presentation_queue,
    VkCommandPool &command_pool)
    : logical_device_(logical_device) {
    // right now keeping it simple, it's not a VBO's business to validate meshes
    // they'll be validated by scene graph
    auto bod = extractBufferObjectData<ValueType, IndexType>(meshes, {true, true});

    BOOST_LOG_TRIVIAL(debug) << "UBO sizes";
    bod.printSizes();

    vertices_size_ = bod.vertices.size();
    normals_size_ = bod.normals.size();
    colors_size_ = bod.colors.size();
    texcoord_size_ = bod.tex_coords.size();

    // Redux to have one buffer that store everything as per vertex data, to evade over-allocations

    buffer_data_.reserve(bod.vertices.size() + bod.normals.size() + bod.colors.size() + bod.tex_coords.size());

    for (size_t i = 0; i < bod.vertices.size() / 3; ++i) {
        auto actual_index = i * 3;
        auto actual_index_color = i * 4;
        auto actual_index_texcoord = i * 2;
        // getting vertex coordinates
        buffer_data_.push_back(bod.vertices[actual_index]);
        buffer_data_.push_back(bod.vertices[actual_index + 1]);
        buffer_data_.push_back(bod.vertices[actual_index + 2]);
        // getting normal coordinates
        buffer_data_.push_back(bod.normals[actual_index]);
        buffer_data_.push_back(bod.normals[actual_index + 1]);
        buffer_data_.push_back(bod.normals[actual_index + 2]);
        // getting color components
        buffer_data_.push_back(bod.colors[actual_index_color]);
        buffer_data_.push_back(bod.colors[actual_index_color + 1]);
        buffer_data_.push_back(bod.colors[actual_index_color + 2]);
        buffer_data_.push_back(bod.colors[actual_index_color + 3]);
        // getting tex coord components
        buffer_data_.push_back(bod.tex_coords[actual_index_texcoord]);
        buffer_data_.push_back(bod.tex_coords[actual_index_texcoord + 1]);
    }

    indices_ = bod.facets;

    BOOST_LOG_TRIVIAL(debug) << buffer_data_.size();

    source_size_ = bod.source_size;
    vertices_byte_size_ = bod.vertices_byte_size;
    normals_byte_size_ = bod.normals_byte_size;
    colors_byte_size_ = bod.colors_byte_size;
    tex_coords_byte_size_ = bod.texcoords_byte_size;

    buffer_size_bytes_ = bod.buffer_byte_size;

    {
        auto res = createVertexBuffer(
            logical_device,
            device,
            vertex_buffer_,
            vertex_buffer_memory_,
            presentation_queue,
            command_pool,
            buffer_data_);
        if (!res)
            throw std::runtime_error("Can't create vertex buffer!");
    }

    {
        auto res = createIndexBuffer(
            logical_device,
            device,
            index_buffer_,
            index_buffer_memory_,
            presentation_queue,
            command_pool,
            indices_);
        if (!res)
            throw std::runtime_error("Can't create index buffer!");
    }

    buffer_set_ = true;
}

VulkanVertexBufferObject::~VulkanVertexBufferObject() {
    deleteBuffer();
}

void VulkanVertexBufferObject::deleteBuffer() {
    if (buffer_set_) {
        vkDestroyBuffer(logical_device_, index_buffer_, nullptr);
        vkFreeMemory(logical_device_, index_buffer_memory_, nullptr);

        vkDestroyBuffer(logical_device_, vertex_buffer_, nullptr);
        vkFreeMemory(logical_device_, vertex_buffer_memory_, nullptr);

        buffer_set_ = false;
    }
}

size_t VulkanVertexBufferObject::size() const noexcept {
    return vertices_size_;
}

size_t VulkanVertexBufferObject::sourceSize() const noexcept {
    return source_size_;
}

size_t VulkanVertexBufferObject::sizeBytes() const noexcept {
    return buffer_size_bytes_;
}

size_t VulkanVertexBufferObject::vertsByteSize() const noexcept {
    return vertices_byte_size_;
}

size_t VulkanVertexBufferObject::normsByteSize() const noexcept {
    return normals_byte_size_;
}

size_t VulkanVertexBufferObject::colorsByteSize() const noexcept {
    return colors_byte_size_;
}

size_t VulkanVertexBufferObject::texCoordByteSize() const noexcept {
    return tex_coords_byte_size_;
}

VulkanVertexBufferObject::BindingDescriptionType VulkanVertexBufferObject::getBindingDescription() {
    BindingDescriptionType ret {};
    ret.binding = 0;
    // 3 for vert; 3 for norm; 4 for RGBA; 2 for texcoord (u;v)
    ret.stride = getStride();
    ret.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return ret;
}

std::array<VkVertexInputAttributeDescription, 4>
VulkanVertexBufferObject::getAttributesDescription() {
    std::array<VkVertexInputAttributeDescription, 4> attribDescr {};

    attribDescr[0].binding = 0;
    attribDescr[0].location = VERTEX_ARRAY_OBJECT_ID;
    attribDescr[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribDescr[0].offset = 0;

    attribDescr[1].binding = 0;
    attribDescr[1].location = NORMAL_ARRAY_OBJECT_ID;
    attribDescr[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribDescr[1].offset = sizeof(ValueType) * 3;

    attribDescr[2].binding = 0;
    attribDescr[2].location = COLOR_ARRAY_OBJECT_ID;
    attribDescr[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attribDescr[2].offset = sizeof(ValueType) * 3 * 2;

    attribDescr[3].binding = 0;
    attribDescr[3].location = TEXCOORD_ARRAY_OBJECT_ID;
    attribDescr[3].format = VK_FORMAT_R32G32_SFLOAT;
    attribDescr[3].offset = sizeof(ValueType) * (3 * 2 + 4);

    return attribDescr;
}

void VulkanVertexBufferObject::bind(VkCommandBuffer command_buffer, const size_t voffset, const size_t ioffset) {
    VkBuffer vertex_buffers[] = {vertex_buffer_};
    VkDeviceSize offsets[] = {voffset};
    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(command_buffer, index_buffer_, ioffset, VulkanIndexType<IndexType>::value);
}

size_t VulkanVertexBufferObject::indicesSize() const noexcept {
    return indices_.size();
}

size_t VulkanVertexBufferObject::getStride() {
    return sizeof(decltype(buffer_data_)::value_type) * (3 + 3 + 4 + 2);
}

VulkanUniformBufferObject::VulkanUniformBufferObject(
    VkDevice logical_device,
    VkPhysicalDevice physical_device,
    VkDescriptorSetLayout desc_set_layout,
    VkImageView image_view,
    VkSampler sampler,
    const size_t image_amount)
    : VulkanUniformBufferObject(logical_device,
                                physical_device,
                                desc_set_layout,
                                std::vector<VkImageView> {image_view},
                                std::vector<VkSampler> {sampler},
                                image_amount) {}


VulkanUniformBufferObject::VulkanUniformBufferObject(
    VkDevice logical_device,
    VkPhysicalDevice physical_device,
    VkDescriptorSetLayout desc_set_layout,
    std::vector<VkImageView> &&image_view,
    std::vector<VkSampler> &&sampler,
    const size_t image_amount)
    : logical_device_ {logical_device}
    , image_view_ {image_view}
    , sampler_ {sampler} {
    if (image_view_.size() != sampler_.size())
        throw std::runtime_error("ImageView and samplers are of different sizes!");
    const auto buffer_size = sizeof(uniform_buffer_object);
    BOOST_LOG_TRIVIAL(trace) << "UBO size " << buffer_size;

    uniform_buffers_.resize(image_amount);
    uniform_buffers_memory_.resize(image_amount);

    const auto local_disaster_cleanup = [this](const size_t starting_index) {
        for (size_t i = starting_index; i >= 0; --i) {
            vkDestroyBuffer(logical_device_, uniform_buffers_[i], nullptr);
            vkFreeMemory(logical_device_, uniform_buffers_memory_[i], nullptr);
        }
    };

    for (size_t i = 0; i < image_amount; ++i) {
        auto res = createBufferImpl(
            logical_device,
            physical_device,
            buffer_size,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniform_buffers_[i],
            uniform_buffers_memory_[i]);

        if (!res) {
            local_disaster_cleanup(i - 1);
            throw std::runtime_error("Couldn't create all needed  uniform buffers");
        }
    }

    {
        auto descriptor_pool_created = createDescriptorPool(logical_device_, image_amount);
        if (!descriptor_pool_created)
            throw std::runtime_error("Can't create descriptor pool!");
    }

    BOOST_LOG_TRIVIAL(debug) << "UBO descriptor pool created!";

    {
        auto descriptor_sets_created = createDescriptorSets(logical_device_, desc_set_layout, image_amount);
        if (!descriptor_sets_created)
            throw std::runtime_error("Can't recreate descriptor sets!");
    }

    buffer_created_ = true;

    BOOST_LOG_TRIVIAL(debug) << "UBO created";
}

VulkanUniformBufferObject::VulkanUniformBufferObject(
    VkDevice logical_device,
    VkPhysicalDevice physical_device,
    VkDescriptorSetLayout desc_set_layout,
    VkSampler global_sampler,
    std::vector<VkImageView> &&image_view,
    const size_t total_textures,
    const size_t image_amount)
    : logical_device_ {logical_device}
    , image_view_ {image_view}
    , global_sampler_ {global_sampler} {
    const auto buffer_size = sizeof(uniform_buffer_object);
    BOOST_LOG_TRIVIAL(trace) << "UBO size " << buffer_size;
    BOOST_LOG_TRIVIAL(trace) << "Image views " << image_view_.size();

    uniform_buffers_.resize(image_amount);
    uniform_buffers_memory_.resize(image_amount);

    const auto local_disaster_cleanup = [this](const size_t starting_index) {
        for (size_t i = starting_index; i >= 0; --i) {
            vkDestroyBuffer(logical_device_, uniform_buffers_[i], nullptr);
            vkFreeMemory(logical_device_, uniform_buffers_memory_[i], nullptr);
        }
    };

    for (size_t i = 0; i < image_amount; ++i) {
        auto res = createBufferImpl(
            logical_device,
            physical_device,
            buffer_size,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniform_buffers_[i],
            uniform_buffers_memory_[i]);

        if (!res) {
            local_disaster_cleanup(i - 1);
            throw std::runtime_error("Couldn't create all needed  uniform buffers");
        }
    }

    {
        auto descriptor_pool_created = createDescriptorPool(logical_device_, image_amount, total_textures);
        if (!descriptor_pool_created)
            throw std::runtime_error("Can't create descriptor pool!");
    }

    BOOST_LOG_TRIVIAL(debug) << "UBO descriptor pool created!";

    {
        auto descriptor_sets_created = createDescriptorSets(
            logical_device_,
            desc_set_layout,
            global_sampler_,
            image_amount,
            total_textures);
        if (!descriptor_sets_created)
            throw std::runtime_error("Can't recreate descriptor sets!");
    }

    buffer_created_ = true;

    BOOST_LOG_TRIVIAL(debug) << "UBO created";
}

void VulkanUniformBufferObject::deleteBuffer() {
    if (buffer_created_) {
        for (size_t i = 0; i < uniform_buffers_.size(); ++i) {
            vkDestroyBuffer(logical_device_, uniform_buffers_[i], nullptr);
            vkFreeMemory(logical_device_, uniform_buffers_memory_[i], nullptr);
        }

        vkDestroyDescriptorPool(logical_device_, descriptor_pool_, nullptr);

        buffer_created_ = false;
    }
}

VulkanUniformBufferObject::~VulkanUniformBufferObject() {
    deleteBuffer();
}

void VulkanUniformBufferObject::update(
    const rcbe::math::Transform &view,
    const rcbe::math::Transform &model,
    const rcbe::math::Matrix4x4f &perspective,
    const uint32_t image_index) {
    uniform_buffer_object ubo {};

    ubo.model = model.matrix().convertUnderlyingValues<float>().transposed();
    ubo.view = view.matrix().convertUnderlyingValues<float>().transposed();
    ubo.proj = perspective.transposed();

    void *data;
    vkMapMemory(logical_device_, uniform_buffers_memory_[image_index], 0, sizeof(ubo), 0, std::addressof(data));
    memcpy(data, std::addressof(ubo), sizeof(ubo));
    vkUnmapMemory(logical_device_, uniform_buffers_memory_[image_index]);
}

bool VulkanUniformBufferObject::createDescriptorPool(
    VkDevice logical_device,
    const size_t image_amount,
    const ssize_t total_textures) {
    std::vector<VkDescriptorPoolSize> pool_sizes {2};
    pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_sizes[0].descriptorCount = static_cast<uint32_t>(image_amount);
    pool_sizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
    pool_sizes[1].descriptorCount = static_cast<uint32_t>(image_amount);
    if (total_textures != -1) {
        VkDescriptorPoolSize ps {};
        ps.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        ps.descriptorCount = static_cast<uint32_t>(image_amount);
        pool_sizes.push_back(ps);
    }

    VkDescriptorPoolCreateInfo pool_info {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
    pool_info.pPoolSizes = pool_sizes.data();
    pool_info.maxSets = static_cast<uint32_t>(image_amount);

    auto res = vkCreateDescriptorPool(
        logical_device,
        std::addressof(pool_info),
        nullptr,
        std::addressof(descriptor_pool_));
    if (res != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(error) << "Can't create descriptor pool!";
        return false;
    }

    return true;
}

bool VulkanUniformBufferObject::createDescriptorSets(
    VkDevice logical_device,
    VkDescriptorSetLayout desc_set_layout,
    const size_t image_amount) {
    std::vector<VkDescriptorSetLayout> layouts(image_amount, desc_set_layout);
    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = descriptor_pool_;
    alloc_info.descriptorSetCount = static_cast<uint32_t>(image_amount);
    alloc_info.pSetLayouts = layouts.data();

    descriptor_sets_.resize(image_amount);
    if (vkAllocateDescriptorSets(logical_device, std::addressof(alloc_info), descriptor_sets_.data()) != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(error) << "Can't allocate descriptor sets";
        return false;
    }

    for (size_t i = 0; i < image_amount; ++i) {
        VkDescriptorBufferInfo buff_info {};
        buff_info.buffer = uniform_buffers_[i];
        buff_info.offset = 0;
        buff_info.range = sizeof(uniform_buffer_object);

        std::vector<VkDescriptorImageInfo> image_infos {};
        image_infos.reserve(image_view_.size());

        for (size_t j = 0; j < image_view_.size(); ++j) {
            VkDescriptorImageInfo image_info {};
            image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_info.imageView = image_view_[j];
            image_info.sampler = sampler_[i];
            image_infos.push_back(image_info);
        }

        std::array<VkWriteDescriptorSet, 2> desc_writes {};

        desc_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc_writes[0].dstSet = descriptor_sets_[i];
        desc_writes[0].dstBinding = 0;
        desc_writes[0].dstArrayElement = 0;
        desc_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        desc_writes[0].descriptorCount = 1;
        desc_writes[0].pBufferInfo = std::addressof(buff_info);
        desc_writes[0].pImageInfo = nullptr;
        desc_writes[0].pTexelBufferView = nullptr;

        desc_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc_writes[1].dstSet = descriptor_sets_[i];
        desc_writes[1].dstBinding = 1;
        desc_writes[1].dstArrayElement = 0;
        desc_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        desc_writes[1].descriptorCount = static_cast<uint32_t>(image_infos.size());
        desc_writes[1].pBufferInfo = nullptr;
        desc_writes[1].pImageInfo = image_infos.data();
        desc_writes[1].pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(
            logical_device,
            static_cast<uint32_t>(desc_writes.size()),
            desc_writes.data(),
            0,
            nullptr);
    }

    return true;
}

bool VulkanUniformBufferObject::createDescriptorSets(
    VkDevice logical_device,
    VkDescriptorSetLayout desc_set_layout,
    VkSampler global_sampler,
    const size_t image_amount,
    const size_t total_textures) {
    const auto total_images_for_textures = std::min(total_textures, image_view_.size());

    if (image_view_.size() != total_images_for_textures) {
        BOOST_LOG_TRIVIAL(warning) << "Too many textures supplied, right now our engine supports only up to "
                                   << total_textures << " textures in fragment shaders!";
    }

    std::vector<VkDescriptorSetLayout> layouts(image_amount, desc_set_layout);
    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = descriptor_pool_;
    alloc_info.descriptorSetCount = static_cast<uint32_t>(image_amount);
    alloc_info.pSetLayouts = layouts.data();

    descriptor_sets_.resize(image_amount);
    if (vkAllocateDescriptorSets(logical_device, std::addressof(alloc_info), descriptor_sets_.data()) != VK_SUCCESS) {
        BOOST_LOG_TRIVIAL(error) << "Can't allocate descriptor sets";
        return false;
    }

    for (size_t i = 0; i < image_amount; ++i) {
        VkDescriptorBufferInfo buff_info {};
        buff_info.buffer = uniform_buffers_[i];
        buff_info.offset = 0;
        buff_info.range = sizeof(uniform_buffer_object);

        std::vector<VkDescriptorImageInfo> image_infos {};
        image_infos.reserve(total_textures);
        for (size_t j = 0; j < total_images_for_textures; ++j) {
            VkDescriptorImageInfo image_info {};
            image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_info.imageView = image_view_[j];
            image_info.sampler = nullptr;
            image_infos.push_back(image_info);
        }

        std::vector<VkWriteDescriptorSet> desc_writes {2};

        desc_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc_writes[0].dstSet = descriptor_sets_[i];
        desc_writes[0].dstBinding = 0;
        desc_writes[0].dstArrayElement = 0;
        desc_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        desc_writes[0].descriptorCount = 1;
        desc_writes[0].pBufferInfo = std::addressof(buff_info);
        desc_writes[0].pImageInfo = nullptr;
        desc_writes[0].pTexelBufferView = nullptr;

        VkDescriptorImageInfo dii {};
        dii.sampler = global_sampler;

        desc_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        desc_writes[1].dstSet = descriptor_sets_[i];
        desc_writes[1].dstBinding = 1;
        desc_writes[1].dstArrayElement = 0;
        desc_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        desc_writes[1].descriptorCount = 1;
        desc_writes[1].pBufferInfo = nullptr;
        desc_writes[1].pImageInfo = std::addressof(dii);
        desc_writes[1].pTexelBufferView = nullptr;

        if (!image_infos.empty()) {
            VkWriteDescriptorSet desc_write {};
            desc_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            desc_write.dstSet = descriptor_sets_[i];
            desc_write.dstBinding = 2;
            desc_write.dstArrayElement = 0;
            desc_write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            desc_write.descriptorCount = static_cast<uint32_t>(image_infos.size());
            desc_write.pBufferInfo = nullptr;
            desc_write.pImageInfo = image_infos.data();
            desc_write.pTexelBufferView = nullptr;
            desc_writes.push_back(desc_write);
        }

        vkUpdateDescriptorSets(
            logical_device,
            static_cast<uint32_t>(desc_writes.size()),
            desc_writes.data(),
            0,
            nullptr);
    }

    return true;
}

void VulkanUniformBufferObject::bind(
    VkCommandBuffer command_buffer,
    VkPipelineLayout pipeline_layout,
    const size_t image_index) {
    vkCmdBindDescriptorSets(
        command_buffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline_layout,
        0, 1,
        std::addressof(descriptor_sets_[image_index]),
        0, nullptr);
}

VkBuffer VulkanUniformBufferObject::getBufferAt(const size_t index) const {
    return uniform_buffers_.at(index);
}

size_t VulkanUniformBufferObject::sizeofBuffObject() const {
    return sizeof(uniform_buffer_object);
}

VkDescriptorSet VulkanUniformBufferObject::getDescriptorSet(const size_t index) const {
    return descriptor_sets_.at(index);
}

}// namespace rdmn::render
