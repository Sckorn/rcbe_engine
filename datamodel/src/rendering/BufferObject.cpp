#include <rcbe-engine/datamodel/rendering/BufferObject.hpp>
#include <rcbe-engine/datamodel/rendering/buffer_object_helpers.hpp>

namespace {
static constexpr size_t VERTEX_ARRAY_OBJECT_ID = 0;
static constexpr size_t NORMAL_ARRAY_OBJECT_ID = 1;
static constexpr size_t COLOR_ARRAY_OBJECT_ID = 2;
static constexpr size_t TEXCOORD_ARRAY_OBJECT_ID = 3;

}// namespace

namespace rcbe::rendering {

VertexBufferObject::VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &id_);
}

VertexBufferObject::VertexArrayObject::~VertexArrayObject() {
    if (id_ != 0) {
        glDisableVertexAttribArray(TEXCOORD_ARRAY_OBJECT_ID);
        glDisableVertexAttribArray(COLOR_ARRAY_OBJECT_ID);
        glDisableVertexAttribArray(NORMAL_ARRAY_OBJECT_ID);
        glDisableVertexAttribArray(VERTEX_ARRAY_OBJECT_ID);
        glDeleteVertexArrays(1, &id_);
    }
}

VertexBufferObject::VertexArrayObject::VertexArrayObject(VertexArrayObject &&other) {
    this->id_ = other.id_;
    this->binded_ = other.binded_;

    other.id_ = 0;
}

VertexBufferObject::VertexArrayObject &VertexBufferObject::VertexArrayObject::operator=(VertexArrayObject &&other) {
    if (this == &other)
        return *this;

    this->id_ = other.id_;
    this->binded_ = other.binded_;

    other.id_ = 0;

    return *this;
}

void VertexBufferObject::VertexArrayObject::bind() const {
    if (!binded_) {
        glBindVertexArray(id_);
        binded_ = true;
    }
}

void VertexBufferObject::VertexArrayObject::unbind() const {
    if (binded_) {
        glBindVertexArray(0);
        binded_ = false;
    }
}

void VertexBufferObject::VertexArrayObject::setData(const StorageType &vertices) const {
    glVertexAttribPointer(
        VERTEX_ARRAY_OBJECT_ID, 3, GL_FLOAT, false,
        sizeof(float) * 3, reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(VERTEX_ARRAY_OBJECT_ID);
}

void VertexBufferObject::VertexArrayObject::setData(const StorageType &vertices, const StorageType &normals) const {
    setData(vertices);
    glVertexAttribPointer(
        NORMAL_ARRAY_OBJECT_ID, 3, GL_FLOAT, false,
        sizeof(float) * 3, reinterpret_cast<void *>(vertices.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(NORMAL_ARRAY_OBJECT_ID);
}

void VertexBufferObject::VertexArrayObject::setData(
    const StorageType &vertices,
    const StorageType &normals,
    const StorageType &colors) const {
    setData(vertices, normals);
    glVertexAttribPointer(
        COLOR_ARRAY_OBJECT_ID, 4, GL_FLOAT, false,
        sizeof(float) * 4, reinterpret_cast<void *>(vertices.size() * sizeof(GLfloat) + normals.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(COLOR_ARRAY_OBJECT_ID);
}

void VertexBufferObject::VertexArrayObject::setData(
    const StorageType &vertices,
    const StorageType &normals,
    const StorageType &colors,
    const StorageType &tex_coords) const {
    setData(vertices, normals, colors);
    glVertexAttribPointer(
        TEXCOORD_ARRAY_OBJECT_ID, 2, GL_FLOAT, false,
        sizeof(float) * 2,
        reinterpret_cast<void *>(
            vertices.size() * sizeof(GLfloat) +
            normals.size() * sizeof(GLfloat) +
            colors.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(TEXCOORD_ARRAY_OBJECT_ID);
}

VertexBufferObject::VertexBufferObject(const std::vector<rcbe::geometry::Mesh> &meshes, bool use_vao) {
    // right now keeping it simple, it's not a VBO's business to validate meshes
    // they'll be validated by scene graph

    auto bod = rdmn::render::extractBufferObjectData<VertexBufferObject::ValueType, GLuint>(meshes);

    vertices_ = std::move(bod.vertices);
    normals_ = std::move(bod.normals);
    colors_ = std::move(bod.colors);
    tex_coords_ = std::move(bod.tex_coords);

    normals_intact_ = bod.normals_intact;

    source_size_ = bod.source_size;
    vertices_byte_size_ = bod.vertices_byte_size;
    normals_byte_size_ = bod.normals_byte_size;
    colors_byte_size_ = bod.colors_byte_size;
    tex_coords_byte_size_ = bod.texcoords_byte_size;

    buffer_size_bytes_ = bod.buffer_byte_size;

    if (use_vao)
        vao_ = VertexArrayObject();

    if (use_vao)
        vao_->bind();

    glGenBuffers(1, &id_);

    bind();

    glBufferData(GL_ARRAY_BUFFER, buffer_size_bytes_, nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_byte_size_, vertices_.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices_byte_size_, normals_byte_size_, normals_.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices_byte_size_ + normals_byte_size_, colors_byte_size_, colors_.data());
    glBufferSubData(
        GL_ARRAY_BUFFER,
        vertices_byte_size_ + normals_byte_size_ + colors_byte_size_,
        tex_coords_byte_size_,
        tex_coords_.data());

    if (use_vao)
        vao_->setData(vertices_, normals_, colors_, tex_coords_);

    if (use_vao)
        vao_->unbind();

    unbind();
}

VertexBufferObject::VertexBufferObject(VertexBufferObject &&other) {
    this->normals_intact_ = other.normals_intact_;
    this->binded_ = other.binded_;

    this->id_ = other.id_;

    this->source_size_ = other.source_size_;
    this->buffer_size_bytes_ = other.buffer_size_bytes_;

    this->vertices_byte_size_ = other.vertices_byte_size_;
    this->normals_byte_size_ = other.normals_byte_size_;
    this->colors_byte_size_ = other.colors_byte_size_;
    this->tex_coords_byte_size_ = other.tex_coords_byte_size_;

    this->vertices_ = std::move(other.vertices_);
    this->normals_ = std::move(other.normals_);
    this->colors_ = std::move(other.colors_);
    this->tex_coords_ = std::move(other.tex_coords_);

    if (other.vao_.has_value())
        this->vao_ = std::move(*(other.vao_));

    other.id_ = 0;
}

VertexBufferObject &VertexBufferObject::operator=(VertexBufferObject &&other) {
    if (this == &other)
        return *this;

    this->normals_intact_ = other.normals_intact_;
    this->binded_ = other.binded_;

    this->id_ = other.id_;

    this->source_size_ = other.source_size_;
    this->buffer_size_bytes_ = other.buffer_size_bytes_;

    this->vertices_byte_size_ = other.vertices_byte_size_;
    this->normals_byte_size_ = other.normals_byte_size_;
    this->colors_byte_size_ = other.colors_byte_size_;
    this->tex_coords_byte_size_ = other.tex_coords_byte_size_;

    this->vertices_ = std::move(other.vertices_);
    this->normals_ = std::move(other.normals_);
    this->colors_ = std::move(other.colors_);
    this->tex_coords_ = std::move(other.tex_coords_);

    if (other.vao_.has_value())
        this->vao_ = std::move(*(other.vao_));

    other.id_ = 0;

    return *this;
}

VertexBufferObject::~VertexBufferObject() {
    if (id_ != 0) {
        deleteBuffer();
    }
}

void VertexBufferObject::deleteBuffer() {
    glDeleteBuffers(1, &id_);
}

void VertexBufferObject::enableState() const {
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void VertexBufferObject::bind() const {
    if (!binded_) {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        binded_ = true;
    }
}

void VertexBufferObject::unbind() const {
    if (binded_) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        binded_ = false;
    }
}

void VertexBufferObject::disableState() const {
    glDisableClientState(GL_VERTEX_ARRAY);// disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

size_t VertexBufferObject::size() const noexcept {
    return vertices_.size();
}

size_t VertexBufferObject::sourceSize() const noexcept {
    return source_size_;
}

size_t VertexBufferObject::sizeBytes() const noexcept {
    return buffer_size_bytes_;
}

size_t VertexBufferObject::vertsByteSize() const noexcept {
    return vertices_byte_size_;
}

size_t VertexBufferObject::normsByteSize() const noexcept {
    return normals_byte_size_;
}

size_t VertexBufferObject::colorsByteSize() const noexcept {
    return colors_byte_size_;
}

VertexBufferObject::RawDataType VertexBufferObject::verticesData() const noexcept {
    return vertices_.data();
}

VertexBufferObject::RawDataType VertexBufferObject::normalsData() const noexcept {
    return normals_.data();
}

VertexBufferObject::RawDataType VertexBufferObject::colorsData() const noexcept {
    return colors_.data();
}

const VertexBufferObject::VertexArrayObject &VertexBufferObject::vao() const {
    if (vao_)
        return *vao_;
    else
        throw std::runtime_error("VAO wasn't set, which most likely means use_vao flag wasn't supplied to ctor");
}

IndexBufferObject::ElementBufferObject::ElementBufferObject() {
    glGenBuffers(1, &id_);
}

IndexBufferObject::ElementBufferObject::~ElementBufferObject() {
    if (id_ != 0) {
        glDeleteBuffers(1, &id_);
    }
}

IndexBufferObject::ElementBufferObject::ElementBufferObject(ElementBufferObject &&other) {
    this->id_ = other.id_;
    this->binded_ = other.binded_;

    other.id_ = 0;
}

IndexBufferObject::ElementBufferObject &IndexBufferObject::ElementBufferObject::operator=(ElementBufferObject &&other) {
    if (this == &other)
        return *this;

    this->id_ = other.id_;
    this->binded_ = other.id_;

    other.id_ = 0;

    return *this;
}

void IndexBufferObject::ElementBufferObject::bind() const {
    if (!binded_) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        binded_ = true;
    }
}

void IndexBufferObject::ElementBufferObject::unbind() const {
    if (binded_) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        binded_ = false;
    }
}

void IndexBufferObject::ElementBufferObject::setData(const StorageType &indices) const {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(typename StorageType::value_type), indices.data(), GL_STATIC_DRAW);
}

IndexBufferObject::IndexBufferObject(const std::vector<rcbe::geometry::Mesh> &meshes, const VertexBufferObject &vbo, bool use_ebo) {
    auto itotal = std::accumulate(meshes.begin(), meshes.end(), 0, [](auto sum, const auto &entry) {
        return sum + entry.facetsSize();
    });

    indices_.reserve(itotal * 3);

    size_t i = 0;
    for (auto &m : meshes) {
        for (const auto &f : m.facets()) {
            indices_.push_back(f.indices[0]);
            indices_.push_back(f.indices[1]);
            indices_.push_back(f.indices[2]);
        }
        ++i;
    }

    glGenBuffers(1, &id_);
    if (use_ebo)
        ebo_ = ElementBufferObject();

    bind();
    if (use_ebo)
        ebo_->bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(decltype(indices_)::value_type), indices_.data(), GL_STATIC_DRAW);
    if (use_ebo)
        ebo_->setData(indices_);

    unbind();
    if (use_ebo)
        ebo_->unbind();
}

IndexBufferObject::~IndexBufferObject() {
    if (id_ != 0) {
        glDeleteBuffers(1, &id_);
    }
}

IndexBufferObject::IndexBufferObject(IndexBufferObject &&other) {
    this->id_ = other.id_;
    this->binded_ = other.binded_;
    this->indices_ = std::move(other.indices_);
    if (other.ebo_.has_value())
        this->ebo_ = std::move(*(other.ebo_));

    other.id_ = 0;
}

IndexBufferObject &IndexBufferObject::operator=(IndexBufferObject &&other) {
    if (this == &other)
        return *this;

    this->id_ = other.id_;
    this->binded_ = other.binded_;
    this->indices_ = std::move(other.indices_);
    if (other.ebo_.has_value())
        this->ebo_ = std::move(*(other.ebo_));

    return *this;
}

void IndexBufferObject::bind() const {
    if (!binded_) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        binded_ = true;
    }
}

void IndexBufferObject::unbind() const {
    if (binded_) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        binded_ = false;
    }
}

size_t IndexBufferObject::size() const noexcept {
    return indices_.size();
}

IndexBufferObject::RawDataType IndexBufferObject::data() const noexcept {
    return indices_.data();
}

const IndexBufferObject::ElementBufferObject &IndexBufferObject::ebo() const {
    if (ebo_)
        return *ebo_;
    else
        throw std::runtime_error("EBO wasn't set, which most likely means use_ebo flag wasn't supplied to ctor");
}

}// namespace rcbe::rendering
