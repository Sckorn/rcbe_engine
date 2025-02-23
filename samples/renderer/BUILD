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
        "@bazel_tools//src/conditions:windows": [],
        "//conditions:default": [
            "-lpthread",
            "-lX11",
            "-lXext",
        ],
    }),
    tags = ["local"],
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
)

### packaging 

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

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
    name = "renderer-sample-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
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
        "rcbe_ticker",
        "rdmn_engine_parser_x3d",
        "rdmn_engine_parser_stl",
        "rdmn_engine_camera",
        "rdmn_engine_renderer",
        "rdmn_engine_common_trivial_logger",
        "nlohmann-json3-dev",
    ] + select({
        "//meta_package:rdmn_gl_linux": ["rdmn_engine_gl_extensions"],
        "//meta_package:rdmn_vulkan_linux": ["vulkan-sdk"],
        "//conditions:default": [],
    }),
    description = "RDMN renderer sample application.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rdmn_renderer_sample",
    version = ENGINE_VERSION_FULL,
)