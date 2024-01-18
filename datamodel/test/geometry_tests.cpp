#include <fstream>

#include <gtest/gtest.h>

#include <rcbe-engine/binary/BinaryBuffer.hpp>
#include <rcbe-engine/datamodel/geometry/BinaryStlFile.hpp>

static constexpr size_t TOTAL_TRIANGLES = 6291;

TEST(GeometryTests, BinaryStlFileTest) {
    std::ifstream ifs {"external/low_poly_wolf_stl/file/LowPolyWolf.stl", std::ios::in | std::ios::binary};
    rcbe::binary::BinaryBuffer bb;
    ifs >> bb;

    rcbe::geometry::binary_stl_header bsh;
    rcbe::binary::from_binary(bb, bsh);
    ASSERT_EQ(bsh.number_triangles, TOTAL_TRIANGLES);

    auto offset = rcbe::geometry::binary_stl_header::SIZE;
    size_t i = 0;
    for (; i < bsh.number_triangles; ++i) {
        rcbe::geometry::binary_stl_chunk bsc {};
        auto bbb = bb.at(offset + rcbe::geometry::binary_stl_chunk::SIZE * i, rcbe::geometry::binary_stl_chunk::SIZE);
        rcbe::binary::from_binary(bbb, bsc);
    }
    ASSERT_EQ(i, TOTAL_TRIANGLES);
}
