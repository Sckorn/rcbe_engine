#include <filesystem>
#include <string>

#include <gtest/gtest.h>

#include <rcbe-engine/parsers/stl/stl_parser.hpp>

namespace {
void _impl(const std::string &file_path, const size_t total_triangles) {
    ASSERT_TRUE(std::filesystem::exists(file_path));
    auto mesh = rcbe::parsers::stl::parse_mesh(file_path);
    ASSERT_EQ(mesh.facetsSize(), total_triangles);
}
}// namespace

TEST(ParsersTest, TestSTLParse) {
    static constexpr size_t TOTAL_TRIANGLES = 6291;
    const std::string file_path = "external/low_poly_wolf_stl/file/LowPolyWolf.stl";
    _impl(file_path, TOTAL_TRIANGLES);
}

TEST(ParsersTest, BeardedYell_15k_Solid) {
    static constexpr size_t TOTAL_TRIANGLES = 30526;
    const std::string file_path = "external/stl_test_data_archive/BeardedYell_15k_Solid.stl";
    _impl(file_path, TOTAL_TRIANGLES);
}

TEST(ParsersTest, BeardedYell_30k_Solid) {
    static constexpr size_t TOTAL_TRIANGLES = 60520;
    const std::string file_path = "external/stl_test_data_archive/BeardedYell_30k_Solid.stl";
    _impl(file_path, TOTAL_TRIANGLES);
}

TEST(ParsersTest, BeardedYell_Low_137k_Solid) {
    static constexpr size_t TOTAL_TRIANGLES = 134026;
    const std::string file_path = "external/stl_test_data_archive/BeardedYell_Low_137k_Solid.stl";
    _impl(file_path, TOTAL_TRIANGLES);
}

TEST(ParsersTest, BeardedYell_Mid_372k_Solid) {
    static constexpr size_t TOTAL_TRIANGLES = 361762;
    const std::string file_path = "external/stl_test_data_archive/BeardedYell_Mid_372k_Solid.stl";
    _impl(file_path, TOTAL_TRIANGLES);
}


TEST(ParsersTest, BeardedYell_High_965k_Solid) {
    static constexpr size_t TOTAL_TRIANGLES = 936928;
    const std::string file_path = "external/stl_test_data_archive/BeardedYell_High_965k_Solid.stl";
    _impl(file_path, TOTAL_TRIANGLES);
}

TEST(ParsersTest, BeardedYell_Highest_2804k_Solid) {
    static constexpr size_t TOTAL_TRIANGLES = 2719830;
    const std::string file_path = "external/stl_test_data_archive/BeardedYell_Highest_2804k_Solid.stl";
    _impl(file_path, TOTAL_TRIANGLES);
}