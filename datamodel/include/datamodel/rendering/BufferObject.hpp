#ifndef RCBE_ENGINE_BUFFEROBJECT_HPP
#define RCBE_ENGINE_BUFFEROBJECT_HPP

#include <vector>
#include <variant>

#include <GL/gl.h>

#include <core/gl_extensions.hpp>

#include <rcbe-engine/datamodel/geometry/Mesh.hpp>

namespace rcbe::rendering {

class VertexBufferObject {
public:
    using ValueType = GLfloat;
    using StorageType = std::vector<ValueType>;
    using RawDataType = const ValueType *;

    explicit VertexBufferObject(const std::vector<rcbe::geometry::Mesh>& meshes);

    ~VertexBufferObject();

    void enableState() const;
    void disableState() const;

    void bind() const;
    void unbind() const;

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] size_t sourceSize() const noexcept;
    [[nodiscard]] size_t sizeBytes() const noexcept;

    [[nodiscard]] size_t vertsByteSize() const noexcept;
    [[nodiscard]] size_t normsByteSize() const noexcept;
    [[nodiscard]] size_t colorsByteSize() const noexcept;

    [[nodiscard]] RawDataType verticesData() const noexcept;
    [[nodiscard]] RawDataType normalsData() const noexcept;
    [[nodiscard]] RawDataType colorsData() const noexcept;

    [[nodiscard]] const std::vector<size_t>& offsets() const noexcept;

private:
    bool normals_intact_ = false;

    GLuint id_ = 0;

    size_t source_size_; // since size of all filled containers (i.e. vertices, normals, colors) should be the same we keep only one size
    size_t buffer_size_bytes_;

    size_t vertices_byte_size_;
    size_t normals_byte_size_;
    size_t colors_byte_size_;

    StorageType vertices_;
    StorageType normals_;
    StorageType colors_;

    std::vector<size_t> vertices_offset_;
};

class IndexBufferObject {
public:
    using ValueType = GLuint;
    using StorageType = std::vector<ValueType>;
    using RawDataType = const ValueType *;

    IndexBufferObject() = delete;
    ~IndexBufferObject();

    IndexBufferObject(const std::vector<rcbe::geometry::Mesh>& meshes, const VertexBufferObject& vbo);

    void bind() const;
    void unbind() const;

    [[nodiscard]] size_t size() const noexcept;

    [[nodiscard]] RawDataType data() const noexcept;

private:
    GLuint id_;
    std::vector<size_t> vertices_offset_;
    StorageType indices_;
};

}

#endif //RCBE_ENGINE_BUFFEROBJECT_HPP
