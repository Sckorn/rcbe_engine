#include <renderer/GLRenderer.hpp>

#include <common/utils/json_utils.hpp>

#include <data_types/math/Vector.hpp>

#include <core/WindowManager.hpp>

#include <parsers/x3d/X3dParser.hpp>

#include <iostream>
#include <chrono>

int main(int argc, char * argv[]) {

    try {
        rcbe::core::WindowManager manager { true };

        auto wconf = rcbe::utils::readFromFile<rcbe::core::WindowConfig>("datamodel/data/system/default_window_config.json");

        auto window = manager.create_window(std::move(wconf));

        auto window_handle = window->start_window_loop_aync();

        auto renderer_conf = rcbe::utils::readFromFile<rcbe::rendering::RendererConfig>("datamodel/data/rendering/default_renderer_config.json");

        rcbe::rendering::GLRenderer renderer { std::move(renderer_conf), window };

        auto renderer_handle = renderer.start_async();

        auto meshes = rcbe::parsers::parseMeshes("parsers/test/resources/corner.x3d");
        auto second_mesh = meshes[0];

        {
            rcbe::math::Quaternion<rcbe::core::EngineScalar> q { 0.0, 0.0, 90.0 };

            rcbe::math::Matrix3x3 rotation { q };
            rcbe::math::Vector3d translation { 0.0, 5.0, 0.0 };

            rcbe::math::Transform t { rotation, translation };

            meshes[0].transform(t);
        }

        {
            rcbe::math::Vector3d translation { 0.0, 5.0, -20.0 };

            rcbe::math::Transform t { {}, translation };
            second_mesh.transform(t);
        }

        std::this_thread::sleep_for(std::chrono::operator""ms(1000));

        renderer.add_object(std::move(meshes[0]));
        renderer.add_object(std::move(second_mesh));

        renderer_handle.wait();
        window_handle.wait();

    } catch (const std::exception& e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}