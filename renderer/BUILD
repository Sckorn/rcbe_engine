cc_library(
    name = "rcbe_renderer",
    srcs = glob(
        ["src/**/*.cpp"],
        exclude = ["src/main.cpp"],
    ),
    hdrs = glob(["include/renderer/**/*.hpp"]),
    includes = ["include"],
    linkopts = [
        "-lpthread",
        "-lOpenGL",
        "-lGL",
        "-lGLX",
        "-lX11",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:engine_datamodel_core",
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_rendering",
        "//datamodel:engine_datamodel_visual",
        "//gl_extensions",
        "//window_manager:rcbe_window_manager",
        "@boost//:log",
        "@nlohmann",
    ],
)

cc_binary(
    name = "test_renderer",
    srcs = ["src/main.cpp"],
    data = [
        "//datamodel:datamodel_rendering_resources",
        "//datamodel:datamodel_system_resources",
        "//parsers:x3d_test_resources",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_renderer",
        "//common:engine_json_utils",
        "//parsers:x3d_parser",
    ],
)