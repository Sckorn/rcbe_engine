#ifndef RCBE_ENGINE_BINARYSTLFILE_HPP
#define RCBE_ENGINE_BINARYSTLFILE_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <vector>

#include <rcbe-engine/binary/BinaryBuffer.hpp>
#include <rcbe-engine/binary/base_serializer.hpp>
#include <rcbe-engine/binary/binary_file_concepts.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

#include <rdmn-engine/public_api.hpp>

namespace rcbe::geometry {
/**
 *
 *
 * uint8_t[80]    – Header                 -     80 bytes
 * uint32_t       – Number of triangles    -      4 bytes
 *
 *
 * */

R_PUBLIC_API struct binary_stl_header {
    static constexpr size_t SIZE = 80 * sizeof(uint8_t) + sizeof(uint32_t);

    static_assert(sizeof(char) == sizeof(uint8_t), "Sizes of char and uint8_t are different");
    uint32_t number_triangles;
};

/**
 *
 *
 * float[3] – Normal vector             - 12 bytes
 * float[3] – Vertex 1                  - 12 bytes
 * float[3] – Vertex 2                  - 12 bytes
 * float[3] – Vertex 3                  - 12 bytes
 * uint16_t – Attribute byte count      -  2 bytes
 *
 *
 * */

R_PUBLIC_API struct binary_stl_chunk {
    static constexpr size_t SIZE = sizeof(float) * 4 * 3 + sizeof(uint16_t);
    rcbe::math::Vector3f normal;
    rcbe::math::Vector3f v1;
    rcbe::math::Vector3f v2;
    rcbe::math::Vector3f v3;
    uint16_t extra_bytes_count;// should be zero, throw otherwise
};

}// namespace rcbe::geometry

namespace rcbe::binary {
R_PUBLIC_API void from_binary(const BinaryBuffer &b, rcbe::geometry::binary_stl_header &bsh);
R_PUBLIC_API void to_binary(BinaryBuffer &b, const rcbe::geometry::binary_stl_header &bsh);

R_PUBLIC_API void from_binary(const BinaryBuffer &b, rcbe::geometry::binary_stl_chunk &bsc);
R_PUBLIC_API void to_binary(BinaryBuffer &b, const rcbe::geometry::binary_stl_chunk &bsc);
}// namespace rcbe::binary

#endif//RCBE_ENGINE_BINARYSTLFILE_HPP
