// TODO: consider making this into a separate pacakge that will be later transformed into engine package

#include <chrono>
#include <memory>
#include <span>// left to clarify that c++20 is used

#include <rcbe-engine/camera/Camera.hpp>
#include <rcbe-engine/core/AbstractInputManager.hpp>
#include <rcbe-engine/core/EditorInputManager.hpp>
#include <rcbe-engine/core/WindowManager.hpp>

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

#include <rdmn-engine/logger/trivial_logger.hpp>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    using rcbe::core::InputManagerImplementation;
    try {
        rcbe::utils::setup_logging();
        rcbe::core::WindowManager manager {hInstance,/*window_class=*/ L"RDMNWindow", nCmdShow};

        /*auto wconf = rcbe::utils::read_from_file<rcbe::core::window_config>(
            "datamodel/data/system/default_window_config.json");*/

        rcbe::core::window_config wconf{};
        wconf.caption = L"RCBE Engine";
        wconf.background_color = rcbe::visual::RGBAColor(0.5, 0.5, 0.5, 1.0);
        wconf.position = decltype(wconf.position)(10.0, 10.0);
        wconf.size = rcbe::core::IntegralDimensions(1024, 768);
        wconf.process_input = true;
        wconf.editor = true;

        RDMN_LOG(RDMN_LOG_DEBUG) << "Before create window!";

        auto window = manager.createWindow(std::move(wconf));

        RDMN_LOG(RDMN_LOG_DEBUG) << "After create window!";

        //auto window_handle = window->startWindowLoopAsync();

        /*auto renderer_conf = rcbe::utils::read_from_file<rcbe::rendering::renderer_config>(
            "datamodel/data/rendering/default_renderer_config.json");*/

        rcbe::rendering::renderer_config render_conf{};
        render_conf.resizable = false;
        render_conf.window_caption = "";
        render_conf.initial_dimensions = { CW_USEDEFAULT, CW_USEDEFAULT };
        render_conf.clear_color = {0.5, 0.5, 0.5, 1.0};

        window->show();

        {
            auto renderer = rdmn::render::make_renderer_ptr(std::move(render_conf), window->getRenderingContext());

            window->setRenderer(std::move(renderer));
        }

        //window->show();

        RDMN_LOG(RDMN_LOG_DEBUG) << "Before starting renderer";

        const auto &renderer = window->getRenderer();

        auto renderer_handle = renderer->startAsync();

        //renderer->waitRendererReady();

        RDMN_LOG(RDMN_LOG_DEBUG) << "Renderer is ready";

        auto meshes = rcbe::parsers::parse_meshes("parsers/test/resources/simple_edge_quad.x3d");
        auto second_mesh = meshes[0];
        auto low_poly_wolf_mesh = rcbe::parsers::stl::parse_mesh("external/low_poly_wolf_stl/file/LowPolyWolf.stl");

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
                break;
            }
        }

#ifdef RDMN_VULKAN
        auto camera_conf = rcbe::utils::read_from_file<rcbe::rendering::camera_config>(
            "datamodel/data/rendering/vulkan_camera_config.json");
#elif defined(RDMN_DIRECTX)
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
        RDMN_LOG(RDMN_LOG_DEBUG) << "Editor Input Manager created in " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " nsecs";
        window->setInputManager(aim);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        RDMN_LOG(RDMN_LOG_DEBUG) << "Meshes should be visible now";
        //renderer->addObjects(std::move(viking_room_objects));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //renderer->addObject(std::move(wolf_object));

        
        window->startWindowLoop();
        renderer->stop();
    } catch (const std::exception &e) {
        RDMN_LOG(RDMN_LOG_ERROR) << "Exception thrown: " << e.what();
        return 1;
    }

    return 0;
}
