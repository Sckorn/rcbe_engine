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
        "//engine_datamodel:engine_datamodel_core",
        "//engine_datamodel:engine_datamodel_math",
        "//engine_datamodel:engine_datamodel_system",
        "//engine_datamodel:engine_datamodel_visual",
        "@boost//:log",
        "@nlohmann",
    ],
)

cc_binary(
    name = "window_test_tool",
    srcs = ["src/test_tool.cpp"],
    data = ["//engine_datamodel:datamodel_system_resources"],
    linkopts = [
        "-lpthread",
        "-lX11",
        "-lXext",
        "-lGLX",
    ],
    deps = [
        ":rcbe_window_manager",
        "//engine_common:engine_json_utils",
    ],
)
