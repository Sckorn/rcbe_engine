#include <gtest/gtest.h>

#include <rcbe-engine/parsers/gltf/gltf_parser.hpp>

#include <iostream>
#include <filesystem>

#ifdef RDMN_VULKAN

TEST(GLTFTests, ParseVikingRoom) {
    std::cout << std::filesystem::current_path().string() << std::endl;
    const auto parsed_objects = rdmn::parse::gltf::parse(
        /*gltf_path=*/"../+http_archive+gltf_test_data_archive/viking_room/scene.gltf", 
        /*bin_path=*/"../+http_archive+gltf_test_data_archive/viking_room/scene.bin"
    );

    ASSERT_EQ(parsed_objects.size(), 103);
}

#endif
