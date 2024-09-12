#include <rcbe-engine/datamodel/geometry/Mesh.hpp>

namespace rcbe::geometry {
R_PUBLIC_API const Mesh::VertexStorage &Mesh::vertices() const noexcept {
    return vertices_;
}

R_PUBLIC_API const Mesh::NormalStorage &Mesh::normals() const noexcept {
    return normals_;
}

R_PUBLIC_API const Mesh::FacetStorage &Mesh::facets() const noexcept {
    return facets_;
}

R_PUBLIC_API const Mesh::TexCoordStorage &Mesh::texCoord() const noexcept {
    return tex_coord_;
}

R_PUBLIC_API size_t Mesh::verticesSize() const noexcept {
    return vertices_.size();
}

R_PUBLIC_API size_t Mesh::normalsSize() const noexcept {
    return normals_.size();
}

R_PUBLIC_API size_t Mesh::facetsSize() const noexcept {
    return facets_.size();
}

R_PUBLIC_API size_t Mesh::texCoordSize() const noexcept {
    return tex_coord_.size();
}

R_PUBLIC_API Mesh::VerticesIterator Mesh::verticesBegin() noexcept {
    return vertices_.begin();
}

R_PUBLIC_API Mesh::VerticesConstIterator Mesh::verticesCbegin() const noexcept {
    return vertices_.cbegin();
}

R_PUBLIC_API Mesh::VerticesIterator Mesh::verticesEnd() noexcept {
    return vertices_.end();
}

R_PUBLIC_API Mesh::VerticesConstIterator Mesh::verticesCend() const noexcept {
    return vertices_.cend();
}

R_PUBLIC_API Mesh::NormalsIterator Mesh::normalsBegin() noexcept {
    return normals_.begin();
}

R_PUBLIC_API Mesh::NormalsConstIterator Mesh::normalsCbegin() const noexcept {
    return normals_.cbegin();
}

R_PUBLIC_API Mesh::NormalsIterator Mesh::normalsEnd() noexcept {
    return normals_.end();
}

R_PUBLIC_API Mesh::NormalsConstIterator Mesh::normalsCend() const noexcept {
    return normals_.cend();
}

R_PUBLIC_API Mesh::FacetsIterator Mesh::facetsBegin() noexcept {
    return facets_.begin();
}

R_PUBLIC_API Mesh::FacetsConstIterator Mesh::facetsCbegin() const noexcept {
    return facets_.cbegin();
}

R_PUBLIC_API Mesh::FacetsIterator Mesh::facetsEnd() noexcept {
    return facets_.end();
}

R_PUBLIC_API Mesh::FacetsConstIterator Mesh::facetsCend() const noexcept {
    return facets_.cend();
}

R_PUBLIC_API Mesh::TexCoordIterator Mesh::texCoordBegin() noexcept {
    return tex_coord_.begin();
}

R_PUBLIC_API Mesh::TexCoordConstIterator Mesh::texCoordCbegin() const noexcept {
    return tex_coord_.cbegin();
}

R_PUBLIC_API Mesh::TexCoordIterator Mesh::texCoordEnd() noexcept {
    return tex_coord_.end();
}

R_PUBLIC_API Mesh::TexCoordConstIterator Mesh::texCoordCend() const noexcept {
    return tex_coord_.cend();
}

R_PUBLIC_API const Mesh::ColorType &Mesh::color() const noexcept {
    return color_;
}

R_PUBLIC_API const Mesh::TransformType &Mesh::getTransform() const noexcept {
    return transform_;
}

R_PUBLIC_API void Mesh::transform(const TransformType &t) {
    transform_ = t * transform_;

    if (!vertices_.empty()) {
        std::transform(vertices_.begin(), vertices_.end(), vertices_.begin(), transform_);
    }

    if (!normals_.empty()) {
        std::transform(normals_.begin(), normals_.end(), normals_.begin(), transform_);
    }
    vertices_trnasformed_ = true;
}

R_PUBLIC_API Mesh::NormalStorage Mesh::normalsOriginal() const {
    if (vertices_trnasformed_) {
        return revertTransform(normals_, transform_);
    } else {
        return normals_;
    }
}

R_PUBLIC_API Mesh::VertexStorage Mesh::verticesOriginal() const {
    if (vertices_trnasformed_) {
        return revertTransform(vertices_, transform_);
    } else {
        return vertices_;
    }
}

R_PUBLIC_API bool Mesh::verticesTransformed() const noexcept {
    return vertices_trnasformed_;
}

R_PUBLIC_API Mesh operator*(const Mesh::TransformType &t, Mesh m) {
    m.transform(t);
    return m;
}
}// namespace rcbe::geometry

namespace nlohmann {
void adl_serializer<rcbe::geometry::Mesh>::to_json(nlohmann::json &j, const rcbe::geometry::Mesh &m) {
    j = {
        {"vertices", m.vertices()},
        {"normals", m.normals()},
        {"facets", m.facets()},
        {"tex_coord", m.texCoord()},
        {"color", m.color()}};
}

void adl_serializer<rcbe::geometry::Mesh>::from_json(const nlohmann::json &j, rcbe::geometry::Mesh &m) {
    auto vertices = j.at("vertices").get<rcbe::geometry::Mesh::VertexStorage>();
    auto normals = j.at("normals").get<rcbe::geometry::Mesh::NormalStorage>();
    auto facets = j.at("facets").get<rcbe::geometry::Mesh::FacetStorage>();
    auto color = j.at("color").get<rcbe::geometry::Mesh::ColorType>();
    auto tex_coord = j.value<rcbe::geometry::Mesh::TexCoordStorage>("tex_coord", rcbe::geometry::Mesh::TexCoordStorage {});

    m = {
        std::make_move_iterator(vertices.begin()), std::make_move_iterator(vertices.end()),
        std::make_move_iterator(normals.begin()), std::make_move_iterator(normals.end()),
        std::make_move_iterator(facets.begin()), std::make_move_iterator(facets.end()),
        std::make_move_iterator(tex_coord.begin()), std::make_move_iterator(tex_coord.end()),
        color};
}
}// namespace nlohmann
