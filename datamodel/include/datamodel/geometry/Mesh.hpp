#ifndef RCBE_MESH_HPP
#define RCBE_MESH_HPP

#include <vector>

#include <rcbe-engine/datamodel/geometry/triangle_indices.hpp>
#include <rcbe-engine/datamodel/math/Transform.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/math/math_constants.hpp>
#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

#include <rdmn-engine/public_api.hpp>

namespace rcbe::geometry {

R_PUBLIC_API class Mesh {
public:

    using VertexType = math::Vector3d;
    using NormalType = math::Vector3d;
    using TriangleType = triangle_indices;
    using TexCoordType = math::Vector2f;/// TODO: consider making Vertices and Normals on float as well @sckorn

    using VertexStorage = std::vector<VertexType>;
    using NormalStorage = std::vector<NormalType>;
    using FacetStorage = std::vector<TriangleType>;
    using TexCoordStorage = std::vector<TexCoordType>;

    using VerticesIterator = VertexStorage::iterator;
    using NormalsIterator = NormalStorage::iterator;
    using FacetsIterator = FacetStorage::iterator;
    using TexCoordIterator = TexCoordStorage::iterator;

    using VerticesConstIterator = VertexStorage::const_iterator;
    using NormalsConstIterator = NormalStorage::const_iterator;
    using FacetsConstIterator = FacetStorage::const_iterator;
    using TexCoordConstIterator = TexCoordStorage::const_iterator;

    using ColorType = visual::RGBAColor;

    using TransformType = math::Transform;

public:

    R_PUBLIC_API Mesh() = default;

    template <typename VertsInIter, typename NormsInIter, typename FacesInIter>
    Mesh(
        VertsInIter vbegin, VertsInIter vend,
        NormsInIter nbegin, NormsInIter nend,
        FacesInIter fbegin, FacesInIter fend,
        const ColorType &color = {0.25, 0.5, 0.5})
        : vertices_(vbegin, vend)
        , normals_(nbegin, nend)
        , facets_(fbegin, fend)
        , color_(color) {}

    template <typename VertsInIter, typename NormsInIter, typename FacesInIter, typename TexCoordInIter>
    Mesh(
        VertsInIter vbegin, VertsInIter vend,
        NormsInIter nbegin, NormsInIter nend,
        FacesInIter fbegin, FacesInIter fend,
        TexCoordInIter tcbegin, TexCoordInIter tcend,
        const ColorType &color = {0.25, 0.5, 0.5})
        : vertices_(vbegin, vend)
        , normals_(nbegin, nend)
        , facets_(fbegin, fend)
        , tex_coord_(tcbegin, tcend)
        , color_(color) {}

    R_PUBLIC_API Mesh(VertexStorage &&v, NormalStorage &&n, FacetStorage &&f, ColorType &&c)
        : vertices_(std::move(v))
        , normals_(std::move(n))
        , facets_(std::move(f))
        , color_(std::move(c)) {}

    R_PUBLIC_API Mesh(VertexStorage &&v, NormalStorage &&n, FacetStorage &&f, TexCoordStorage &&tc, ColorType &&c)
        : vertices_(std::move(v))
        , normals_(std::move(n))
        , facets_(std::move(f))
        , tex_coord_(std::move(tc))
        , color_(std::move(c)) {}

    // TODO: find a way to make this obsolete
    R_PUBLIC_API Mesh(const Mesh &other) = default;
    R_PUBLIC_API Mesh &operator=(const Mesh &other) = default;

    R_PUBLIC_API Mesh(Mesh &&other) = default;
    R_PUBLIC_API Mesh &operator=(Mesh &&other) = default;

    [[nodiscard]] R_PUBLIC_API const VertexStorage &vertices() const noexcept;
    [[nodiscard]] R_PUBLIC_API VertexStorage verticesOriginal() const;
    [[nodiscard]] R_PUBLIC_API const NormalStorage &normals() const noexcept;
    [[nodiscard]] R_PUBLIC_API NormalStorage normalsOriginal() const;
    [[nodiscard]] R_PUBLIC_API const FacetStorage &facets() const noexcept;
    [[nodiscard]] R_PUBLIC_API const TexCoordStorage &texCoord() const noexcept;

    [[nodiscard]] R_PUBLIC_API size_t verticesSize() const noexcept;
    [[nodiscard]] R_PUBLIC_API size_t normalsSize() const noexcept;
    [[nodiscard]] R_PUBLIC_API size_t facetsSize() const noexcept;
    [[nodiscard]] R_PUBLIC_API size_t texCoordSize() const noexcept;

    R_PUBLIC_API VerticesIterator verticesBegin() noexcept;
    R_PUBLIC_API VerticesConstIterator verticesCbegin() const noexcept;

    R_PUBLIC_API VerticesIterator verticesEnd() noexcept;
    R_PUBLIC_API VerticesConstIterator verticesCend() const noexcept;

    R_PUBLIC_API NormalsIterator normalsBegin() noexcept;
    R_PUBLIC_API NormalsConstIterator normalsCbegin() const noexcept;

    R_PUBLIC_API NormalsIterator normalsEnd() noexcept;
    R_PUBLIC_API NormalsConstIterator normalsCend() const noexcept;

    R_PUBLIC_API FacetsIterator facetsBegin() noexcept;
    R_PUBLIC_API FacetsConstIterator facetsCbegin() const noexcept;

    R_PUBLIC_API FacetsIterator facetsEnd() noexcept;
    R_PUBLIC_API FacetsConstIterator facetsCend() const noexcept;

    R_PUBLIC_API TexCoordIterator texCoordBegin() noexcept;
    R_PUBLIC_API TexCoordConstIterator texCoordCbegin() const noexcept;

    R_PUBLIC_API TexCoordIterator texCoordEnd() noexcept;
    R_PUBLIC_API TexCoordConstIterator texCoordCend() const noexcept;

    [[nodiscard]] R_PUBLIC_API const ColorType &color() const noexcept;

    [[nodiscard]] R_PUBLIC_API const TransformType &getTransform() const noexcept;

    R_PUBLIC_API void transform(const TransformType &t);

    [[nodiscard]] R_PUBLIC_API bool verticesTransformed() const noexcept;

    friend R_PUBLIC_API Mesh operator*(const TransformType &t, Mesh m);

private:

    template <typename Storage>
    Storage revertTransform(const Storage &s, const TransformType &t) const {
        Storage storage {};
        if (!s.empty()) {
            storage.reserve(s.size());
            std::transform(s.begin(), s.end(), std::back_inserter(storage), t.inversed());
        }
        return storage;
    }

    VertexStorage vertices_;
    NormalStorage normals_;
    FacetStorage facets_;
    TexCoordStorage tex_coord_;

    ColorType color_;

    TransformType transform_;

    bool vertices_trnasformed_ = false;
};
}// namespace rcbe::geometry

namespace nlohmann {
template <>
struct adl_serializer<rcbe::geometry::Mesh> {
    static void to_json(nlohmann::json &j, const rcbe::geometry::Mesh &m);
    static void from_json(const nlohmann::json &j, rcbe::geometry::Mesh &m);
};
}// namespace nlohmann


#endif