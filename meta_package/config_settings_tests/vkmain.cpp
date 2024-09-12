#include <iostream>

#include <rcbe-engine/engine_config.hpp>

auto main(int argc, char *argv[]) -> int {
    std::cout << "Engine version: " << rdmn::core::getEngineVersionString() << std::endl;
    std::cout << "Project URL: " << rdmn::core::getProjectUrlString() << std::endl;
    std::cout << "Project maintainer: " << rdmn::core::getMaintainerString() << std::endl;
    std::cout << "I am supposed to be Vulkan rasterizer!" << std::endl;

#ifdef R_VULKAN_WND
    std::cout << "I run under windows!" << std::endl;
#endif

#ifdef R_VULKAN_LNX
    std::cout << "I run under linux!" << std::endl;
#endif

    return 0;
}