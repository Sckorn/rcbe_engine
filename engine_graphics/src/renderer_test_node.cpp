#include <iostream>
#include <thread>
#include <chrono>

#include <core/Renderer.hpp>

#include <parsers/x3d/X3dParser.hpp>

int main(int argc, char * argv[])
{
    using namespace std::chrono_literals;
    rcbe::core::rendering::RendererConfig config {};
    rcbe::core::rendering::Renderer renderer { std::move(config) };
    auto renderer_handle = std::async(std::launch::async, [&renderer]() {
        renderer.launch();
    });

    std::this_thread::sleep_for(10s);

    auto meshes = rcbe::parsers::parseMeshes("parsers/test/resources/corner.x3d");
    renderer.addObject(std::move(meshes[0]));

    renderer_handle.wait();
    return 0;
}