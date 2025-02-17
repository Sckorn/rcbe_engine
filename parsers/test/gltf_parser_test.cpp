#include <gtest/gtest.h>

#include <rcbe-engine/parsers/gltf/gltf_parser.hpp>

#ifdef RDMN_VULKAN

TEST(GLTFTests, ParseVikingRoom) {
    const auto parsed_objects = rdmn::parse::gltf::parse(
        /*gltf_path=*/"external/gltf_test_data_archive/viking_room/scene.gltf", 
        /*bin_path=*/"external/gltf_test_data_archive/viking_room/scene.bin"
    );

    ASSERT_EQ(parsed_objects.size(), 103);
}

#endif
