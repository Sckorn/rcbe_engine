#include <rcbe-engine/datamodel/geometry/BinaryStlFile.hpp>

namespace rcbe::binary {
void from_binary(const BinaryBuffer &b, rcbe::geometry::binary_stl_header &bsh) {
    auto it = b.constBegin();
    std::advance(it, rcbe::geometry::binary_stl_header::SIZE);
    bsh.number_triangles = b.at( std::prev(it, sizeof(uint32_t)), sizeof(uint32_t)).get<uint32_t>().return_value;
}

void to_binary(BinaryBuffer &b, const rcbe::geometry::binary_stl_header &bsh) {
    std::array<unsigned char, 80> tmp{};
    b = {tmp, bsh.number_triangles};
}

void from_binary(const BinaryBuffer &b, rcbe::geometry::binary_stl_chunk &bsc) {
    auto offset = b.constBegin();
    std::tie(bsc.normal, offset) = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    std::tie(bsc.v1, offset) = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    std::tie(bsc.v2, offset) = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    std::tie(bsc.v3, offset) = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    std::tie(bsc.extra_bytes_count, offset) = b.at(offset, sizeof(uint16_t)).get<uint16_t>();
}

void to_binary(BinaryBuffer &b, const rcbe::geometry::binary_stl_chunk &bsc) {
    b = {bsc.normal, bsc.v1, bsc.v2, bsc.v3, bsc.extra_bytes_count};
}
}