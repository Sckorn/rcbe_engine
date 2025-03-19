#include <rcbe-engine/datamodel/geometry/BinaryStlFile.hpp>

namespace rcbe::binary {
template <>
R_PUBLIC_API void from_binary(const BinaryBuffer &b, rcbe::geometry::binary_stl_header &bsh) {
    bsh.number_triangles = b.at(rcbe::geometry::binary_stl_header::SIZE - sizeof(uint32_t), sizeof(uint32_t)).get<uint32_t>();
}

template <>
R_PUBLIC_API void to_binary(BinaryBuffer &b, const rcbe::geometry::binary_stl_header &bsh) {
    std::array<unsigned char, 80> tmp {};
    b = {tmp, bsh.number_triangles};
}

template <>
R_PUBLIC_API void from_binary(const BinaryBuffer &b, rcbe::geometry::binary_stl_chunk &bsc) {
    size_t offset = 0;
    bsc.normal = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    bsc.v1 = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    bsc.v2 = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    bsc.v3 = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    bsc.extra_bytes_count = b.at(offset, sizeof(uint16_t)).get<uint16_t>();
}

template <>
R_PUBLIC_API void to_binary(BinaryBuffer &b, const rcbe::geometry::binary_stl_chunk &bsc) {
    b = {bsc.normal, bsc.v1, bsc.v2, bsc.v3, bsc.extra_bytes_count};
}
}// namespace rcbe::binary