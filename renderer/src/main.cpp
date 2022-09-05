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
#include <rcbe-engine/datamodel/core/CoreObject.hpp>
#include <rcbe-engine/datamodel/visual/texture_types.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/datamodel/rendering/Material.hpp>
#include <rcbe-engine/core/gl_extensions.hpp>

int main(int argc, char * argv[]) {
    struct some_object {};

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

        renderer->waitRendererReady();

        BOOST_LOG_TRIVIAL(debug) << "Renderer is ready";

        rcbe::visual::texture_config tex_conf;
        tex_conf.component_order = decltype(tex_conf.component_order)::RGBA;
        tex_conf.filtering_type = decltype(tex_conf.filtering_type)::linear;
        tex_conf.texture_type = decltype(tex_conf.texture_type)::texture_2d;
        tex_conf.wrapping_type = decltype(tex_conf.wrapping_type)::repeat;

        std::vector<rcbe::rendering::Material::ShaderArguments> shader_args;
        shader_args.reserve(2);
        shader_args.push_back({"datamodel/data/rendering/shaders/default.vert", rcbe::rendering::ShaderType::vertex});
        shader_args.push_back({"datamodel/data/rendering/shaders/default.frag", rcbe::rendering::ShaderType::fragment});

        std::vector<rcbe::rendering::Material::TextureArguments> texture_args;
        texture_args.reserve(2);
        texture_args.push_back({"external/brick_wall_texture/file/brick_wall_texture.tga", tex_conf, true});
        texture_args.push_back({"external/awesomeface_texture/file/awesomeface_texture.tga", tex_conf, true});

        rcbe::rendering::Material material(
                std::move(shader_args),
                std::move(texture_args),
                true
        );

        rcbe::core::CoreObject first_corner_wall{some_object{}};
        rcbe::core::CoreObject second_corner_wall{some_object{}};
        rcbe::core::CoreObject wolf{some_object{}};

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

            auto material_copy = material;

            first_corner_wall.addComponent<rcbe::geometry::Mesh>(std::move(meshes[0]));
            first_corner_wall.addComponent<rcbe::rendering::Material>(std::move(material_copy));
        }

        {
            rcbe::math::Vector3d translation { 0.0, 0.0, -25.0 };

            rcbe::math::Transform t { {}, translation };
            second_mesh.transform(t);

            auto material_copy = material;
            second_corner_wall.addComponent<rcbe::geometry::Mesh>(std::move(second_mesh));
            second_corner_wall.addComponent<rcbe::rendering::Material>(std::move(material_copy));
        }

        {
            rcbe::math::Vector3d translation { -10.0, 0.0, -10.0 };
            rcbe::math::Transform t { {}, translation };

            low_poly_wolf_mesh.transform(t);

            std::vector<rcbe::rendering::Material::ShaderArguments> shader_args_;
            shader_args_.reserve(2);
            shader_args_.push_back({"datamodel/data/rendering/shaders/default.vert", rcbe::rendering::ShaderType::vertex});
            shader_args_.push_back({"datamodel/data/rendering/shaders/wolf.frag", rcbe::rendering::ShaderType::fragment});

            rcbe::rendering::Material mat(std::move(shader_args_), {}, true);

            wolf.addComponent<rcbe::geometry::Mesh>(std::move(low_poly_wolf_mesh));
            wolf.addComponent<rcbe::rendering::Material>(std::move(mat));
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

        renderer->addObject(std::move(first_corner_wall));
        renderer->addObject(std::move(second_corner_wall));
        renderer->addObject(std::move(wolf));

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
