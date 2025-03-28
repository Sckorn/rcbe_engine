#ifndef RCBE_GEOMETRY_HPP
#define RCBE_GEOMETRY_HPP

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include <nlohmann/json.hpp>

#include <rcbe-engine/fundamentals/types.hpp>

namespace rcbe::geometry {

struct R_PUBLIC_API triangle_indices {
    using ValueType = size_t;

    triangle_indices() {
        indices.fill(0);
        tex_coords_indices.fill(0);
    }

    triangle_indices(const ValueType v1, const ValueType v2, const ValueType v3) noexcept
        : indices({v1, v2, v3}) {
        tex_coords_indices.fill(0);
    }

    triangle_indices(
        const ValueType v1,
        const ValueType v2,
        const ValueType v3,
        const ValueType t1,
        const ValueType t2,
        const ValueType t3) noexcept
        : indices({v1, v2, v3})
        , tex_coords_indices({t1, t2, t3}) {
    }

    std::array<ValueType, 3> indices;
    std::array<ValueType, 3> tex_coords_indices;
};
}// namespace rcbe::geometry

R_PUBLIC_API std::istream &operator>>(std::istream &is, rcbe::geometry::triangle_indices &tri);

namespace nlohmann {
template <>
struct R_PUBLIC_API adl_serializer<rcbe::geometry::triangle_indices> {
    static void to_json(nlohmann::json &j, const rcbe::geometry::triangle_indices &t);
    static void from_json(const nlohmann::json &j, rcbe::geometry::triangle_indices &t);
};
}// namespace nlohmann

#endif