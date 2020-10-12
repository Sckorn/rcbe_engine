cc_library(
    name = "rcbe_input_manager",
    srcs = glob(
        ["src/**/*.cpp"],
        exclude = ["src/main.cpp"],
    ),
    hdrs = glob(["include/**/*.hpp"]),
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
        "//common:engine_stack_utils",
        "//common:rcbe_delegate",
        "//datamodel:engine_datamodel_system",
        "//gl_extensions",
        "//traits:rcbe_traits",
        "@boost//:bimap",
        "@boost//:log",
    ],
)
