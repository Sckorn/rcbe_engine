#include <rcbe-engine/datamodel/geometry/BinaryStlFile.hpp>

namespace rcbe::binary {
void from_binary(const BinaryBuffer &b, rcbe::geometry::binary_stl_header &bsh) {
    bsh.number_triangles = b.at(rcbe::geometry::binary_stl_header::SIZE - sizeof(uint32_t), sizeof(uint32_t)).get<uint32_t>();
}

void to_binary(BinaryBuffer &b, const rcbe::geometry::binary_stl_header &bsh) {
    std::array<unsigned char, 80> tmp{};
    b = {tmp, bsh.number_triangles};
}

void from_binary(const BinaryBuffer &b, rcbe::geometry::binary_stl_chunk &bsc) {
    size_t offset = 0;
    bsc.normal = b.at(0, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    offset += sizeof(float) * 3;
    bsc.v1 = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    offset += sizeof(float) * 3;
    bsc.v2 = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    offset += sizeof(float) * 3;
    bsc.v3 = b.at(offset, sizeof(float) * 3).get<rcbe::math::Vector3f>();
    offset += sizeof(float) * 3;
    bsc.extra_bytes_count = b.at(offset, sizeof(uint16_t)).get<uint16_t>();
}

void to_binary(BinaryBuffer &b, const rcbe::geometry::binary_stl_chunk &bsc) {
    b = {bsc.normal, bsc.v1, bsc.v2, bsc.v3, bsc.extra_bytes_count};
}
}