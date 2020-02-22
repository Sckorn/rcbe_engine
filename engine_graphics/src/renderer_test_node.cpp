#include <iostream>
#include <thread>
#include <chrono>

#include <core/Renderer.hpp>

#include <data_types/math/Quaternion.hpp>

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

    rcbe::math::Quaternion<rcbe::core::EngineScalar> q { 0.0, 0.0, 90.0 };

    rcbe::math::Matrix3x3 rotation { q };
    rcbe::math::Vector3d translation { 0.0, 5.0, 0.0 };

    rcbe::math::Transform t { rotation, translation };

    meshes[0].transform(t);

    renderer.addObject(std::move(meshes[0]));

    renderer_handle.wait();
    return 0;
}