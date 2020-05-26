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
        "//engine_datamodel:engine_datamodel_core",
        "//engine_datamodel:engine_datamodel_math",
        "//engine_datamodel:engine_datamodel_rendering",
        "//engine_datamodel:engine_datamodel_visual",
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
        "//engine_datamodel:datamodel_rendering_resources",
        "//engine_datamodel:datamodel_system_resources",
        "//parsers:x3d_test_resources",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_renderer",
        "//engine_common:engine_json_utils",
        "//parsers:x3d_parser",
    ],
)
