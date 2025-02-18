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
        "//renderer:rdmn_renderer",
        "//camera:rcbe_engine_camera",
        "//common:engine_json_utils",
        "//common:rcbe_profiling",
        "//datamodel:engine_datamodel_math",
        "//parsers:rcbe_stl_parser",
        "//parsers:rdmn_gltf_parser",
        "//parsers:rdmn_tga_parser",
        "//parsers:x3d_parser",
        "//window_manager:rcbe_window_manager",
        "//common:rdmn_trivial_logger",
        "@nlohmann//:nlohmann_json",
    ],
)

### packaging 

## Bin

pkg_tar(
    name = "renderer-sample-data",
    srcs = [
        ":renderer_sample",
    ],
    mode = "0644",
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
    name = "renderer-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":renderer-package-data",
    depends = [
        "rcbe_common_utils_json_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_rendering_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_visual_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_window_manager_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_gl_extensions_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_ticker_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_parsers_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_camera_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rdmn_trivial_logger_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "nlohmann-json3-dev",
    ],
    description = "RDMN renderer sample application.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rdmn_renderer_sample",
    version = ENGINE_VERSION_FULL,
)