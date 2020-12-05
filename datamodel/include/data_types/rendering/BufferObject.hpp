#ifndef RCBE_ENGINE_BUFFEROBJECT_HPP
#define RCBE_ENGINE_BUFFEROBJECT_HPP

#include <vector>
#include <variant>

#include <GL/gl.h>

#include <core/gl_extensions.hpp>

#include <data_types/geometry/Mesh.hpp>

namespace rcbe::rendering {

class VertexBufferObject {
public:
    using value_type = GLfloat;
    using storage_type = std::vector<value_type>;
    using raw_data_type = const value_type *;

    explicit VertexBufferObject(const std::vector<rcbe::geometry::Mesh>& meshes);

    ~VertexBufferObject();

    void enable_state() const;
    void disable_state() const;

    void bind() const;
    void unbind() const;

    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t source_size() const;
    [[nodiscard]] size_t size_bytes() const;

    [[nodiscard]] size_t verts_byte_size() const;
    [[nodiscard]] size_t norms_byte_size() const;
    [[nodiscard]] size_t colors_byte_size() const;

    [[nodiscard]] raw_data_type vertices_data() const;
    [[nodiscard]] raw_data_type normals_data() const;
    [[nodiscard]] raw_data_type colors_data() const;

    [[nodiscard]] const std::vector<size_t>& offsets() const;

private:
    bool normals_intact_ = false;

    GLuint id_ = 0;

    size_t source_size_; // since size of all filled containers (i.e. vertices, normals, colors) should be the same we keep only one size
    size_t buffer_size_bytes_;

    size_t vertices_byte_size_;
    size_t normals_byte_size_;
    size_t colors_byte_size_;

    storage_type vertices_;
    storage_type normals_;
    storage_type colors_;

    std::vector<size_t> vertices_offset_;
};

class IndexBufferObject {
public:
    using value_type = GLuint;
    using storage_type = std::vector<value_type>;
    using raw_data_type = const value_type *;

    IndexBufferObject() = delete;
    ~IndexBufferObject();

    IndexBufferObject(const std::vector<rcbe::geometry::Mesh>& meshes, const VertexBufferObject& vbo);

    void bind() const;
    void unbind() const;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] raw_data_type data() const;

private:
    GLuint id_;
    std::vector<size_t> vertices_offset_;
    storage_type indices_;
};

}

#endif //RCBE_ENGINE_BUFFEROBJECT_HPP
