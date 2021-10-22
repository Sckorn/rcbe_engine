#ifndef RCBE_MESH_HPP
#define RCBE_MESH_HPP

#include <vector>

#include <rcbe-engine/datamodel/math/math_constants.hpp>
#include <rcbe-engine/datamodel/math/Transform.hpp>
#include <rcbe-engine/datamodel/geometry/triangle_indices.hpp>
#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::geometry
{
class Mesh
{
public:
    using VertexType = math::Vector3d;
    using NormalType = math::Vector3d;
    using TriangleType = triangle_indices;
    using TexCoordType = math::Vector2d;

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
    Mesh() = default;

    template <typename VertsInIter, typename NormsInIter, typename FacesInIter>
    Mesh(
        VertsInIter vbegin, VertsInIter vend,
        NormsInIter nbegin, NormsInIter nend,
        FacesInIter fbegin, FacesInIter fend,
        const ColorType &color = {0.25, 0.5, 0.5}
    )
    :
    vertices_(vbegin, vend)
    , normals_(nbegin, nend)
    , facets_(fbegin, fend)
    , color_(color)
    {}

    template <typename VertsInIter, typename NormsInIter, typename FacesInIter, typename TexCoordInIter>
    Mesh(
            VertsInIter vbegin, VertsInIter vend,
            NormsInIter nbegin, NormsInIter nend,
            FacesInIter fbegin, FacesInIter fend,
            TexCoordInIter tcbegin, TexCoordInIter tcend,
            const ColorType &color = {0.25, 0.5, 0.5}
    )
    :
    vertices_(vbegin, vend)
    , normals_(nbegin, nend)
    , facets_(fbegin, fend)
    , tex_coord_(tcbegin, tcend)
    , color_(color)
    {}

    Mesh(VertexStorage &&v, NormalStorage &&n, FacetStorage &&f, ColorType &&c)
    :
    vertices_(std::move(v))
    , normals_(std::move(n))
    , facets_(std::move(f))
    , color_(std::move(c))
    {}

    Mesh(VertexStorage &&v, NormalStorage &&n, FacetStorage &&f, TexCoordStorage &&tc, ColorType &&c)
    :
    vertices_(std::move(v))
    , normals_(std::move(n))
    , facets_(std::move(f))
    , tex_coord_(std::move(tc))
    , color_(std::move(c))
    {}

    // TODO: find a way to make this obsolete
    Mesh(const Mesh &other) = default;
    Mesh &operator=(const Mesh &other) = default;

    Mesh(Mesh &&other) = default;
    Mesh &operator=(Mesh &&other) = default;

    [[nodiscard]] const VertexStorage &vertices() const noexcept;
    [[nodiscard]] VertexStorage verticesOriginal() const;
    [[nodiscard]] const NormalStorage &normals() const noexcept;
    [[nodiscard]] NormalStorage normalsOriginal() const;
    [[nodiscard]] const FacetStorage &facets() const noexcept;
    [[nodiscard]] const TexCoordStorage &texCoord() const noexcept;

    [[nodiscard]] size_t verticesSize() const noexcept;
    [[nodiscard]] size_t normalsSize() const noexcept;
    [[nodiscard]] size_t facetsSize() const noexcept;
    [[nodiscard]] size_t texCoordSize() const noexcept;

    VerticesIterator verticesBegin() noexcept;
    VerticesConstIterator verticesCbegin() const noexcept;

    VerticesIterator verticesEnd() noexcept;
    VerticesConstIterator verticesCend() const noexcept;

    NormalsIterator normalsBegin() noexcept;
    NormalsConstIterator normalsCbegin() const noexcept;

    NormalsIterator normalsEnd() noexcept;
    NormalsConstIterator normalsCend() const noexcept;

    FacetsIterator facetsBegin() noexcept;
    FacetsConstIterator facetsCbegin() const noexcept;

    FacetsIterator facetsEnd() noexcept;
    FacetsConstIterator facetsCend() const noexcept;

    TexCoordIterator texCoordBegin() noexcept;
    TexCoordConstIterator texCoordCbegin() const noexcept;

    TexCoordIterator texCoordEnd() noexcept;
    TexCoordConstIterator texCoordCend() const noexcept;

    [[nodiscard]] const ColorType &color() const noexcept;

    [[nodiscard]] const TransformType &getTransform() const noexcept;

    void transform(const TransformType &t);

    [[nodiscard]] bool verticesTransformed() const noexcept;

    friend Mesh operator*(const TransformType &t, Mesh m);
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
} // namespace rcbe::geometry

namespace nlohmann
{
template <>
struct adl_serializer<rcbe::geometry::Mesh>
{
    static void to_json(nlohmann::json &j, const rcbe::geometry::Mesh &m);
    static void from_json(const nlohmann::json &j, rcbe::geometry::Mesh &m);
};
}


#endif