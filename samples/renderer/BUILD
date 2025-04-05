cc_binary(
    name = "renderer_sample",
    srcs = select({
        "@bazel_tools//src/conditions:windows": ["src/winmain.cpp"],
        "//conditions:default": ["src/linuxmain.cpp"], #TODO: @sckorn @radameon we prolly should handle linux case explicitly
    }), 
    data = [
        "//datamodel:datamodel_rendering_resources",
        "//datamodel:datamodel_system_resources",
        "//parsers:x3d_test_resources",
        "@awesomeface_texture//file",
        "@brick_wall_texture//file",
        "@gltf_test_data_archive//:gltf_tests_archive",
        "@low_poly_wolf_stl//file",
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": [
            "/D_UNICODE", 
            "/DUNICODE", 
            "/Zc:wchar_t",
        ],
        "//conditions:default": [],
    }),
    linkopts = select({
        "@bazel_tools//src/conditions:windows": ["/ENTRY:wWinMainCRTStartup", "User32.lib"],
        "//conditions:default": [
            "-lpthread",
            "-lX11",
            "-lXext",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//renderer:rdmn_engine_renderer",
        "//camera:rdmn_engine_camera",
        "//common:rdmn_engine_common_json_utils",
        "//common:rdmn_engine_common_profiling",
        "//datamodel:rdmn_engine_datamodel_math",
        "//parsers:rdmn_engine_parser_stl",
        "//parsers:rdmn_engine_parser_gltf",
        "//parsers:rdmn_engine_parser_tga",
        "//parsers:rdmn_engine_parser_x3d",
        "//window_manager:rdmn_engine_window_manager",
        "//common:rdmn_engine_common_trivial_logger",
        "@nlohmann//:nlohmann_json",
    ],
    dynamic_deps = [
        "//renderer:rdmn_engine_renderer_shared",
        "//camera:rdmn_engine_camera_shared",
        "//common:rdmn_engine_common_json_utils_shared",
        "//datamodel:rdmn_engine_datamodel_math_shared",
        "//datamodel:rdmn_engine_datamodel_geometry_shared",
        "//datamodel:rdmn_engine_datamodel_visual_shared",
        "//datamodel:rdmn_engine_datamodel_system_shared",
        "//datamodel:rdmn_engine_datamodel_rendering_shared",
        "//parsers:rdmn_engine_parser_stl_shared",
        "//parsers:rdmn_engine_parser_gltf_shared",
        "//parsers:rdmn_engine_parser_tga_shared",
        "//parsers:rdmn_engine_parser_x3d_shared",
        "//window_manager:rdmn_engine_window_manager_shared",
        "//input_manager:rdmn_engine_input_manager_shared",
        "//common:rdmn_engine_common_trivial_logger_shared",
    ],
)

### packaging 

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("@rules_wix//:rules.bzl", "pkg_msi")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")

## Bin

pkg_tar(
    name = "renderer-sample-data",
    srcs = [
        ":renderer_sample",
    ],
    mode = "0755",
    package_dir = "/opt/radameon/",
)

pkg_tar(
    name = "renderer-sample-x3d-data",
    srcs = [
        "//parsers:x3d_test_resources",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/parsers/test/resources/",
)

pkg_tar(
    name = "renderer-sample-facetex-data",
    srcs = [
        "@awesomeface_texture//file",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/external/awesomeface_texture/file/",
)

pkg_tar(
    name = "renderer-sample-walltex-data",
    srcs = [
        "@brick_wall_texture//file",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/external/brick_wall_texture/file/",
)

pkg_tar(
    name = "renderer-sample-gltf-scene-data",
    srcs = [
        "@gltf_test_data_archive//:gltf_scene_files",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/external/gltf_test_data_archive/viking_room/",
)

pkg_tar(
    name = "renderer-sample-gltf-tex-data",
    srcs = [
        "@gltf_test_data_archive//:gltf_tex_files",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/external/gltf_test_data_archive/viking_room/textures/",
)

pkg_tar(
    name = "renderer-sample-stl-data",
    srcs = [
        "@low_poly_wolf_stl//file",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/external/low_poly_wolf_stl/file/",
)

pkg_tar(
    name = "renderer-sample-shaders-data",
    srcs = [
        "//datamodel:datamodel_spv_shaders",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/datamodel/data/rendering/shaders/",
)

pkg_tar(
    name = "renderer-sample-renderer-configs",
    srcs = [
        "//datamodel:datamodel_renderer_config",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/datamodel/data/rendering/",
)

pkg_tar(
    name = "renderer-sample-camera-configs",
    srcs = [
        "//datamodel:datamodel_camera_config",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/datamodel/data/rendering/",
)

pkg_tar(
    name = "renderer-sample-window-configs",
    srcs = [
        "//datamodel:datamodel_window_config",
    ],
    mode = "0644",
    package_dir = "/opt/radameon/datamodel/data/system/",
)

pkg_tar(
    name = "renderer-sample-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":renderer-sample-shaders-data",
        ":renderer-sample-renderer-configs",
        ":renderer-sample-camera-configs",
        ":renderer-sample-window-configs",
        ":renderer-sample-x3d-data",
        ":renderer-sample-facetex-data",
        ":renderer-sample-walltex-data",
        ":renderer-sample-gltf-scene-data",
        ":renderer-sample-gltf-tex-data",
        ":renderer-sample-stl-data",
        ":renderer-sample-data",
    ],
)

pkg_deb(
    name = "renderer-sample-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":renderer-sample-package-data",
    depends = [
        "rdmn_engine_common_json_utils",
        "rdmn_engine_datamodel_core",
        "rdmn_engine_datamodel_math",
        "rdmn_engine_datamodel_rendering",
        "rdmn_engine_datamodel_visual",
        "rdmn_engine_window_manager",
        "rdmn_engine_ticker",
        "rdmn_engine_parser_x3d",
        "rdmn_engine_parser_stl",
        "rdmn_engine_parser_tga",
        "rdmn_engine_parser_gltf",
        "rdmn_engine_camera",
        "rdmn_engine_renderer",
        "rdmn_engine_common_trivial_logger",
    ] + select({
        "//meta_package:rdmn_gl_linux": ["rdmn_engine_gl_extensions"],
        "//meta_package:rdmn_vulkan_linux": ["vulkan-sdk"],
        "//conditions:default": [],
    }),
    description = "RDMN renderer sample application.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_renderer_sample".format(ENGINE_BUILD_TARGET_PREFIX),
    version = ENGINE_VERSION_FULL,
)

pkg_msi(
  name = "rdmn_engine_renderer_sample.msi",
  src = "packaging/renderer_sample.wxs",
  deps = [
    ":renderer_sample",
    "//renderer:rdmn_engine_renderer_shared",
    "//camera:rdmn_engine_camera_shared",
    "//common:rdmn_engine_common_json_utils_shared",
    "//datamodel:rdmn_engine_datamodel_math_shared",
    "//datamodel:rdmn_engine_datamodel_geometry_shared",
    "//datamodel:rdmn_engine_datamodel_visual_shared",
    "//datamodel:rdmn_engine_datamodel_system_shared",
    "//datamodel:rdmn_engine_datamodel_rendering_shared",
    "//parsers:rdmn_engine_parser_stl_shared",
    "//parsers:rdmn_engine_parser_gltf_shared",
    "//parsers:rdmn_engine_parser_tga_shared",
    "//parsers:rdmn_engine_parser_x3d_shared",
    "//window_manager:rdmn_engine_window_manager_shared",
    "//input_manager:rdmn_engine_input_manager_shared",
    "//common:rdmn_engine_common_trivial_logger_shared",
    #data 
    "//datamodel:datamodel_rendering_resources",
    "//datamodel:datamodel_system_resources",
    "//parsers:x3d_test_resources",
    "@awesomeface_texture//file",
    "@brick_wall_texture//file",
    "@gltf_test_data_archive//:gltf_tests_archive",
    "@low_poly_wolf_stl//file",
  ],
  arch = "x64",
  exts = [
    "WixUiExtension",
  ],
)