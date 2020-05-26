cc_library(
    name = "rcbe_window_manager",
    srcs = glob(
        [
            "src/**/*.cpp",
            "src/**/*.hpp",
        ],
        exclude = ["src/test_tool.cpp"],
    ),
    hdrs = glob(["include/**/*.hpp"]),
    includes = ["include"],
    linkopts = [
        "-lpthread",
        "-lX11",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:engine_datamodel_core",
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_system",
        "//datamodel:engine_datamodel_visual",
        "@boost//:log",
        "@nlohmann",
    ],
)

cc_binary(
    name = "window_test_tool",
    srcs = ["src/test_tool.cpp"],
    data = ["//datamodel:datamodel_system_resources"],
    linkopts = [
        "-lpthread",
        "-lX11",
        "-lXext",
        "-lGLX",
    ],
    deps = [
        ":rcbe_window_manager",
        "//common:engine_json_utils",
    ],
)
