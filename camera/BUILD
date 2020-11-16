cc_library(
    name = "rcbe_engine_camera",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_rendering",
    ],
)
