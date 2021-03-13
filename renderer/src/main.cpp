// TODO: consider making this into a separate pacakge that will be later transformed into engine package

#include <chrono>
#include <memory>
#include <span> // left to clarify that c++20 is used

#include <rcbe-engine/renderer/GLRenderer.hpp>
#include <rcbe-engine/camera/Camera.hpp>

#include <rcbe-engine/utils/json_utils.hpp>

#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/rendering/renderer_config.hpp>
#include <rcbe-engine/datamodel/rendering/camera_config.hpp>

#include <rcbe-engine/core/WindowManager.hpp>

#include <rcbe-engine/parsers/x3d/x3d_parser.hpp>
#include <rcbe-engine/parsers/stl/stl_parser.hpp>

#include <rcbe-engine/utils/output_utils.hpp>
#include <rcbe-engine/core/AbstractInputManager.hpp>
#include <rcbe-engine/core/EditorInputManager.hpp>

int main(int argc, char * argv[]) {
    using rcbe::core::InputManagerImplementation;
    try {
        rcbe::utils::setup_logging();
        rcbe::core::WindowManager manager { true };

        auto wconf = rcbe::utils::read_from_file<rcbe::core::window_config>(
                "datamodel/data/system/default_window_config.json");

        auto window = manager.createWindow(std::move(wconf));

        auto window_handle = window->startWindowLoopAync();

        auto renderer_conf = rcbe::utils::read_from_file<rcbe::rendering::renderer_config>(
                "datamodel/data/rendering/default_renderer_config.json");

        window->show();

        {
            auto renderer = rcbe::rendering::make_renderer_ptr(std::move(renderer_conf), window->getRenderingContext());

            window->setRenderer(std::move(renderer));
        }

        const auto& renderer = window->getRenderer();

        auto renderer_handle = renderer->startAsync();

        auto meshes = rcbe::parsers::parse_meshes("parsers/test/resources/corner.x3d");
        auto second_mesh = meshes[0];

        auto low_poly_wolf_mesh = rcbe::parsers::stl::parse_mesh("external/low_poly_wolf_stl/file/LowPolyWolf.stl");

        {
            rcbe::math::yaw y(rcbe::math::deg(0));
            rcbe::math::pitch p(rcbe::math::deg(0));
            rcbe::math::roll r(rcbe::math::deg(90));
            rcbe::math::Quaternion<rcbe::core::EngineScalar> q { y, p, r };

            rcbe::math::Matrix3x3 rotation { q };
            rcbe::math::Vector3d translation { 0.0, 0.0, 0.0 };

            rcbe::math::Transform t { rotation, translation };

            meshes[0].transform(t);
        }

        {
            rcbe::math::Vector3d translation { 0.0, 0.0, -25.0 };

            rcbe::math::Transform t { {}, translation };
            second_mesh.transform(t);
        }

        {
            rcbe::math::Vector3d translation { -10.0, 0.0, -10.0 };
            rcbe::math::Transform t { {}, translation };

            low_poly_wolf_mesh.transform(t);
        }

        auto camera_conf = rcbe::utils::read_from_file<rcbe::rendering::camera_config>(
                "datamodel/data/rendering/default_camera_config.json");
        auto camera = rcbe::rendering::make_camera(window->getRenderingContext(), camera_conf);
        auto start =  std::chrono::steady_clock::now();
        auto aim = std::make_shared<rcbe::core::AbstractInputManager>(rcbe::core::EditorInputManager::create(
                window->getRenderingContext(), camera));
        auto end =  std::chrono::steady_clock::now();
        BOOST_LOG_TRIVIAL(debug) << "Editor Input Manager created in " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " nsecs";
        window->setInputManager(aim);

        std::this_thread::sleep_for(std::chrono::milliseconds (1000));
        BOOST_LOG_TRIVIAL(debug) << "Meshes should be visible now";

        renderer->addObject(std::move(meshes[0]));
        renderer->addObject(std::move(second_mesh));
        renderer->addObject(std::move(low_poly_wolf_mesh));

        renderer_handle.wait();
        window_handle.wait();
    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Exception thrown: " << e.what();
        return 1;
    }

#ifdef  RCBE_DEBUG_MODE
    std::array<int, 2> arr = {0, 1};
    std::span s{arr};
#endif

    return 0;
}
