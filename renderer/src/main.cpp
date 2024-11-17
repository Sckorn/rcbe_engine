// TODO: consider making this into a separate pacakge that will be later transformed into engine package

#include <chrono>
#include <memory>
#include <span>// left to clarify that c++20 is used

#include <rcbe-engine/camera/Camera.hpp>
#include <rcbe-engine/core/AbstractInputManager.hpp>
#include <rcbe-engine/core/EditorInputManager.hpp>
#include <rcbe-engine/core/WindowManager.hpp>
#ifdef RDMN_OPENGL
#include <rcbe-engine/core/gl_extensions.hpp>
#endif
#include <rcbe-engine/datamodel/core/CoreObject.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/rendering/Material.hpp>
#include <rcbe-engine/datamodel/rendering/RasterizerTexture.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/datamodel/rendering/camera_config.hpp>
#include <rcbe-engine/datamodel/rendering/rasterizer_texture_types.hpp>
#include <rcbe-engine/datamodel/rendering/renderer_config.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include <rcbe-engine/datamodel/visual/texture_types.hpp>
#include <rcbe-engine/fundamentals/convinience.hpp>
#include <rcbe-engine/parsers/gltf/gltf_parser.hpp>
#include <rcbe-engine/parsers/stl/stl_parser.hpp>
#include <rcbe-engine/parsers/tga/tga_parser.hpp>
#include <rcbe-engine/parsers/x3d/x3d_parser.hpp>
#include <rcbe-engine/renderer/Renderer.hpp>
#include <rcbe-engine/utils/json_utils.hpp>
#include <rcbe-engine/utils/output_utils.hpp>

#include <boost/log/trivial.hpp>

int main(int argc, char *argv[]) {
    using rcbe::core::InputManagerImplementation;
    try {
        rcbe::utils::setup_logging();
        rcbe::core::WindowManager manager {true};

        auto wconf = rcbe::utils::read_from_file<rcbe::core::window_config>(
            "datamodel/data/system/default_window_config.json");

        auto window = manager.createWindow(std::move(wconf));

        auto window_handle = window->startWindowLoopAsync();

        auto renderer_conf = rcbe::utils::read_from_file<rcbe::rendering::renderer_config>(
            "datamodel/data/rendering/default_renderer_config.json");

        window->show();

        {
            auto renderer = rdmn::render::make_renderer_ptr(std::move(renderer_conf), window->getRenderingContext());

            window->setRenderer(std::move(renderer));
        }

        const auto &renderer = window->getRenderer();

        auto renderer_handle = renderer->startAsync();

        renderer->waitRendererReady();

        BOOST_LOG_TRIVIAL(debug) << "Renderer is ready";

#ifdef RDMN_OPENGL
        struct some_object {};
        rcbe::visual::texture_config tex_conf;
        tex_conf.component_order = decltype(tex_conf.component_order)::RGBA;

        rdmn::render::rasterizer_texture_config raster_tex_conf;
        raster_tex_conf.filtering_type = decltype(raster_tex_conf.filtering_type)::linear;
        raster_tex_conf.texture_type = decltype(raster_tex_conf.texture_type)::texture_2d;
        raster_tex_conf.wrapping_type = decltype(raster_tex_conf.wrapping_type)::repeat;
        raster_tex_conf.image_config = tex_conf;

        std::vector<rcbe::rendering::Material::ShaderArguments> shader_args;
        shader_args.reserve(2);
        shader_args.push_back({"datamodel/data/rendering/shaders/default.vert", rdmn::render::ShaderType::vertex});
        shader_args.push_back({"datamodel/data/rendering/shaders/default.frag", rdmn::render::ShaderType::fragment});

        auto brick_wall_tex = rcbe::visual::make_tex_ptr(
            "external/brick_wall_texture/file/brick_wall_texture.tga", rdmn::parse::tga::parse);
        auto awesome_face_tex = rcbe::visual::make_tex_ptr(
            "external/awesomeface_texture/file/awesomeface_texture.tga", rdmn::parse::tga::parse);

        std::vector<rcbe::rendering::Material::TextureArguments> texture_args;
        texture_args.reserve(2);
        texture_args.push_back({brick_wall_tex, raster_tex_conf});
        texture_args.push_back({awesome_face_tex, raster_tex_conf});

        rcbe::rendering::Material::MaterialConfig config {
            std::move(shader_args),
            std::move(texture_args)};
        rcbe::rendering::Material material(
            std::move(config),
            true);
        auto mat_co_ptr = std::shared_ptr<rcbe::core::CoreObject>(new rcbe::core::CoreObject {std::move(material)});

        rcbe::core::CoreObject first_corner_wall {some_object {}};
        rcbe::core::CoreObject second_corner_wall {some_object {}};
        rcbe::core::CoreObject wolf {some_object {}};
#endif

        auto meshes = rcbe::parsers::parse_meshes("parsers/test/resources/simple_edge_quad.x3d");
        auto second_mesh = meshes[0];
        auto low_poly_wolf_mesh = rcbe::parsers::stl::parse_mesh("external/low_poly_wolf_stl/file/LowPolyWolf.stl");

#ifdef RDMN_OPENGL
        {
            rcbe::math::yaw y(rcbe::math::deg(0));
            rcbe::math::pitch p(rcbe::math::deg(0));
            rcbe::math::roll r(rcbe::math::deg(90));
            rcbe::math::Quaternion<rcbe::core::EngineScalar> q {y, p, r};

            rcbe::math::Matrix3x3 rotation {q};
            rcbe::math::Vector3d translation {0.0, 0.0, 0.0};

            rcbe::math::Transform t {rotation, translation};

            meshes[0].transform(t);

            auto material_copy = mat_co_ptr;

            first_corner_wall.addComponent<rcbe::geometry::Mesh>(std::move(meshes[0]));
            first_corner_wall.addComponent<rcbe::rendering::Material>(material_copy);
        }

        {
            rcbe::math::Vector3d translation {0.0, 0.0, -25.0};

            rcbe::math::Transform t {{}, translation};
            second_mesh.transform(t);

            auto material_copy = mat_co_ptr;
            second_corner_wall.addComponent<rcbe::geometry::Mesh>(std::move(second_mesh));
            second_corner_wall.addComponent<rcbe::rendering::Material>(material_copy);
        }

        {
            rcbe::math::Vector3d translation {-10.0, 0.0, -10.0};
            rcbe::math::Transform t {{}, translation};

            low_poly_wolf_mesh.transform(t);

            std::vector<rcbe::rendering::Material::ShaderArguments> shader_args_;
            shader_args_.reserve(2);
            shader_args_.push_back({"datamodel/data/rendering/shaders/default.vert", rdmn::render::ShaderType::vertex});
            shader_args_.push_back({"datamodel/data/rendering/shaders/wolf.frag", rdmn::render::ShaderType::fragment});

            rcbe::rendering::Material::MaterialConfig mconfig {
                std::move(shader_args_),
                {}};
            rcbe::rendering::Material mmat(std::move(mconfig), true);

            wolf.addComponent<rcbe::geometry::Mesh>(std::move(low_poly_wolf_mesh));
            wolf.addComponent<rcbe::rendering::Material>(std::move(mmat));
        }
#endif

#ifdef RDMN_VULKAN

        auto viking_room_objects = rdmn::parse::gltf::parse(
            "external/gltf_test_data_archive/viking_room/scene.gltf",
            "external/gltf_test_data_archive/viking_room/scene.bin");

        std::string name = "wolf";
        rcbe::core::CoreObject wolf_object {std::move(name)};
        {
            rcbe::visual::texture_config tex_conf;
            tex_conf.component_order = decltype(tex_conf.component_order)::RGBA;

            rdmn::render::rasterizer_texture_config raster_tex_conf;
            raster_tex_conf.filtering_type = decltype(raster_tex_conf.filtering_type)::linear;
            raster_tex_conf.texture_type = decltype(raster_tex_conf.texture_type)::texture_2d;
            raster_tex_conf.wrapping_type = decltype(raster_tex_conf.wrapping_type)::repeat;
            raster_tex_conf.image_config = tex_conf;

            std::vector<rcbe::rendering::Material::ShaderArguments> shader_args;
            shader_args.reserve(2);
            shader_args.push_back(
                {"datamodel/data/rendering/shaders/default_vulkan.vert.spv",
                 rdmn::render::ShaderType::vertex,
                 rdmn::render::ShaderState::precompiled});
            shader_args.push_back(
                {"datamodel/data/rendering/shaders/default_vulkan.frag.spv",
                 rdmn::render::ShaderType::fragment,
                 rdmn::render::ShaderState::precompiled});

            auto brick_wall_tex = rcbe::visual::make_tex_ptr(
                "external/brick_wall_texture/file/brick_wall_texture.tga", rdmn::parse::tga::parse);
            auto awesome_face_tex = rcbe::visual::make_tex_ptr(
                "external/awesomeface_texture/file/awesomeface_texture.tga", rdmn::parse::tga::parse);

            rcbe::visual::VisualTextureSet vts {brick_wall_tex, awesome_face_tex};

            std::vector<rcbe::rendering::Material::TextureArguments> texture_args;
            texture_args.reserve(2);
            texture_args.push_back({brick_wall_tex, raster_tex_conf});
            texture_args.push_back({awesome_face_tex, raster_tex_conf});

            rcbe::rendering::Material::MaterialConfig config {
                std::move(shader_args),
                std::move(texture_args)};
            rcbe::rendering::Material material(
                std::move(config));

            rcbe::math::Transform trn {
                rcbe::math::Matrix4x4 {
                    1.0, 0., 0., 1.5,
                    0., 0., 1., 0.,
                    0., -1., 0., 0.,
                    0., 0., 0., 1.}};

            second_mesh.transform(trn);

            wolf_object.addComponent<rcbe::geometry::Mesh>(std::move(second_mesh));
            wolf_object.addComponent<rcbe::rendering::Material>(std::move(material));
            wolf_object.addComponent<rcbe::visual::VisualTextureSet>(std::move(vts));
        }

        {
            std::unordered_set<rcbe::visual::TexturePtr> tex_cache;
            rcbe::rendering::Material::MaterialConfig mc {
                {{"datamodel/data/rendering/shaders/viking_room.vert.spv",
                  rdmn::render::ShaderType::vertex,
                  rdmn::render::ShaderState::precompiled},
                 {"datamodel/data/rendering/shaders/viking_room.frag.spv",
                  rdmn::render::ShaderType::fragment,
                  rdmn::render::ShaderState::precompiled}}};

            rcbe::rendering::Material m(std::move(mc));
            auto mat_ptr = std::make_shared<rcbe::core::CoreObject>(std::move(m));

            for (size_t i = 0; i < viking_room_objects.size(); ++i) {
                auto &c = viking_room_objects[i];
                const auto &tex_component = c.getComponent<rdmn::render::RasterizerTexture>();
                if (!tex_component)
                    continue;
                auto &rt = tex_component->as<rdmn::render::RasterizerTexture>();
                const auto &vtex_ptr = rt.getVisualTexturePtr();

                const auto it = tex_cache.find(vtex_ptr);
                if (it == tex_cache.end()) {
                    const auto [vtex_it, res] = tex_cache.insert(vtex_ptr);

                    auto &mat = mat_ptr->as<rcbe::rendering::Material>();
                    auto inserted_tex_it = mat.pushTexture(
                        std::make_shared<rdmn::render::RasterizerTexture>(
                            rt.getConfig(),
                            rt.getVisualTexturePtr()));

                    if (!c.hasComponent<rcbe::rendering::Material>())
                        c.addComponent<rcbe::rendering::Material>(mat_ptr);
                } else {
                    if (!c.hasComponent<rcbe::rendering::Material>())
                        c.addComponent<rcbe::rendering::Material>(mat_ptr);
                }
            }
        }

#endif

#ifdef RDMN_VULKAN
        auto camera_conf = rcbe::utils::read_from_file<rcbe::rendering::camera_config>(
            "datamodel/data/rendering/vulkan_camera_config.json");
#elif defined(RDMN_OPENGL)
        auto camera_conf = rcbe::utils::read_from_file<rcbe::rendering::camera_config>(
            "datamodel/data/rendering/default_camera_config.json");
#elif defined(RDMN_DIRECTX) && defined(_WIN32)
        auto camera_conf = rcbe::utils::read_from_file<rcbe::rendering::camera_config>(
            "datamodel/data/rendering/default_camera_config.json");
#else
        static_assert(false, RASTERIZER_NOT_SET_ERROR_MSG);
#endif

        auto camera = rcbe::rendering::make_camera(window->getRenderingContext(), camera_conf);
        auto start = std::chrono::steady_clock::now();
        auto aim = std::make_shared<rcbe::core::AbstractInputManager>(rcbe::core::EditorInputManager::create(
            window->getRenderingContext(), camera));
        auto end = std::chrono::steady_clock::now();
        BOOST_LOG_TRIVIAL(debug) << "Editor Input Manager created in " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " nsecs";
        window->setInputManager(aim);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        BOOST_LOG_TRIVIAL(debug) << "Meshes should be visible now";

#ifdef RDMN_OPENGL
        renderer->addObject(std::move(first_corner_wall));
        renderer->addObject(std::move(second_corner_wall));
        renderer->addObject(std::move(wolf));
#endif

#ifdef RDMN_VULKAN
        renderer->addObjects(std::move(viking_room_objects));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        renderer->addObject(std::move(wolf_object));
#endif

        renderer_handle.wait();
        window_handle.wait();
    } catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Exception thrown: " << e.what();
        return 1;
    }

#ifdef RCBE_DEBUG_MODE
    std::array<int, 2> arr = {0, 1};
    std::span s {arr};
#endif

    return 0;
}
