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
        "//common:engine_output_utils",
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