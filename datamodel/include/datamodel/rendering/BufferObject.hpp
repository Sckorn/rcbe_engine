#ifndef RCBE_ENGINE_BUFFEROBJECT_HPP
#define RCBE_ENGINE_BUFFEROBJECT_HPP

#include <optional>
#include <variant>
#include <vector>

#include <GL/gl.h>

#include <rdmn-engine/public_api.hpp>

#include <rcbe-engine/core/gl_extensions.hpp>
#include <rcbe-engine/datamodel/geometry/Mesh.hpp>

namespace rcbe::rendering {

class VertexBufferObject {
public:

    using ValueType = GLfloat;
    using StorageType = std::vector<ValueType>;
    using RawDataType = const ValueType *;

    class VertexArrayObject {
    public:

        R_PUBLIC_API VertexArrayObject();
        R_PUBLIC_API ~VertexArrayObject();

        R_PUBLIC_API VertexArrayObject(const VertexArrayObject &other) = delete;
        R_PUBLIC_API VertexArrayObject &operator=(const VertexArrayObject &other) = delete;

        R_PUBLIC_API VertexArrayObject(VertexArrayObject &&other);
        R_PUBLIC_API VertexArrayObject &operator=(VertexArrayObject &&other);

        R_PUBLIC_API void bind() const;
        R_PUBLIC_API void unbind() const;
        R_PUBLIC_API void setData(const StorageType &vertices) const;
        R_PUBLIC_API void setData(const StorageType &vertices, const StorageType &normals) const;
        R_PUBLIC_API void setData(const StorageType &vertices, const StorageType &normals, const StorageType &colors) const;
        R_PUBLIC_API void setData(
            const StorageType &vertices,
            const StorageType &normals,
            const StorageType &colors,
            const StorageType &tex_coords) const;

    private:

        unsigned int id_ = 0;
        mutable bool binded_ = false;
    };

    R_PUBLIC_API explicit VertexBufferObject(const std::vector<rcbe::geometry::Mesh> &meshes, bool use_vao = false);

    R_PUBLIC_API VertexBufferObject(const VertexBufferObject &other) = delete;
    R_PUBLIC_API VertexBufferObject &operator=(const VertexBufferObject &other) = delete;

    R_PUBLIC_API VertexBufferObject(VertexBufferObject &&other);
    R_PUBLIC_API VertexBufferObject &operator=(VertexBufferObject &&other);

    R_PUBLIC_API ~VertexBufferObject();

    R_PUBLIC_API void deleteBuffer();

    R_PUBLIC_API void enableState() const;
    R_PUBLIC_API void disableState() const;

    R_PUBLIC_API void bind() const;
    R_PUBLIC_API void unbind() const;

    [[nodiscard]] R_PUBLIC_API size_t size() const noexcept;
    [[nodiscard]] R_PUBLIC_API size_t sourceSize() const noexcept;
    [[nodiscard]] R_PUBLIC_API size_t sizeBytes() const noexcept;

    [[nodiscard]] R_PUBLIC_API size_t vertsByteSize() const noexcept;
    [[nodiscard]] R_PUBLIC_API size_t normsByteSize() const noexcept;
    [[nodiscard]] R_PUBLIC_API size_t colorsByteSize() const noexcept;

    [[nodiscard]] R_PUBLIC_API RawDataType verticesData() const noexcept;
    [[nodiscard]] R_PUBLIC_API RawDataType normalsData() const noexcept;
    [[nodiscard]] R_PUBLIC_API RawDataType colorsData() const noexcept;

    [[nodiscard]] R_PUBLIC_API const VertexArrayObject &vao() const;

private:

    bool normals_intact_ = false;
    mutable bool binded_ = false;

    GLuint id_ = 0;

    ssize_t source_size_;// since size of all filled containers (i.e. vertices, normals, colors) should be the same we keep only one size
    ssize_t buffer_size_bytes_;

    ssize_t vertices_byte_size_;
    ssize_t normals_byte_size_;
    ssize_t colors_byte_size_;
    ssize_t tex_coords_byte_size_;

    StorageType vertices_;
    StorageType normals_;
    StorageType colors_;
    StorageType tex_coords_;

    std::optional<VertexArrayObject> vao_ {};
};

class IndexBufferObject {
public:

    using ValueType = GLuint;
    using StorageType = std::vector<ValueType>;
    using RawDataType = const ValueType *;

    class ElementBufferObject {
    public:

        R_PUBLIC_API ElementBufferObject();
        R_PUBLIC_API ~ElementBufferObject();

        R_PUBLIC_API ElementBufferObject(const ElementBufferObject &other) = delete;
        R_PUBLIC_API ElementBufferObject &operator=(const ElementBufferObject &other) = delete;

        R_PUBLIC_API ElementBufferObject(ElementBufferObject &&other);
        R_PUBLIC_API ElementBufferObject &operator=(ElementBufferObject &&other);

        R_PUBLIC_API void bind() const;
        R_PUBLIC_API void unbind() const;
        R_PUBLIC_API void setData(const StorageType &indices) const;

    private:

        unsigned int id_ = 0;
        mutable bool binded_ = false;
    };

    R_PUBLIC_API IndexBufferObject() = delete;
    R_PUBLIC_API ~IndexBufferObject();

    R_PUBLIC_API IndexBufferObject(const std::vector<rcbe::geometry::Mesh> &meshes, const VertexBufferObject &vbo, bool use_ebo = false);

    R_PUBLIC_API IndexBufferObject(const IndexBufferObject &other) = delete;
    R_PUBLIC_API IndexBufferObject &operator=(const IndexBufferObject &other) = delete;

    R_PUBLIC_API IndexBufferObject(IndexBufferObject &&other);
    R_PUBLIC_API IndexBufferObject &operator=(IndexBufferObject &&other);

    R_PUBLIC_API void bind() const;
    R_PUBLIC_API void unbind() const;

    [[nodiscard]] R_PUBLIC_API size_t size() const noexcept;

    [[nodiscard]] R_PUBLIC_API RawDataType data() const noexcept;

    [[nodiscard]] R_PUBLIC_API const ElementBufferObject &ebo() const;

private:

    GLuint id_;
    StorageType indices_;
    mutable bool binded_ = false;

    std::optional<ElementBufferObject> ebo_ = {};
};

}// namespace rcbe::rendering

#endif//RCBE_ENGINE_BUFFEROBJECT_HPP
