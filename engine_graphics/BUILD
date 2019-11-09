cc_library(
    name = "engine_graphics",
    srcs = glob([
        "src/**/*.cpp",
    ],
    exclude = ["src/renderer_test_node.cpp"],
    ),
    hdrs = glob([
        "include/**/*.hpp",
        "include/**/*.h",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "//engine_datamodel:engine_datamodel_math",
        "//engine_datamodel:engine_datamodel_rendering",
        "//engine_datamodel:engine_datamodel_visual",
        "@nlohmann",
    ],
    copts = ["-Iexternal/nlohmann/include"],
)

cc_binary(
    name = "renderer_test_node",
    srcs = ["src/renderer_test_node.cpp"],
    deps = [
        ":engine_graphics",
        "@boost//:filesystem",
        "@boost//:system",
        "//parsers:x3d_parser",
    ],
    data = ["//parsers:x3d_test_resources"],
    linkopts = ["-lpthread", "-lOpenGL", "-lGL", "-lGLU",  "-lglut", "-lGLX", "-lGLEW"],
    visibility = ["//visibility:public"],
)