cc_library(
    name = "engine_datamodel_core",
    hdrs = glob([
        "include/data_model_config.hpp",
        "include/data_types/core/**/*.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "engine_datamodel_math",
    hdrs = glob([
        "include/data_types/math/**/*.cpp",
    ]),
    srcs = glob([
        "src/math/**/*.cpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
    ], 
)

cc_library(
    name = "engine_datamodel_geometry",
    hdrs = glob([
        "include/data_types/geometry/**/*.hpp",
    ]),
    srcs = glob(["src/geometry/**/*.cpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
    ], 
)

cc_library(
    name = "engine_datamodel_gui",
    hdrs = glob([
        "include/data_types/gui/**/*.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
    ], 
)

cc_library(
    name = "engine_datamodel_rendering",
    hdrs = glob([
        "include/data_types/rendering/**/*.hpp",
    ]),
    srcs = glob([
        "src/rendering/**/*.cpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        ":engine_datamodel_geometry",
        ":engine_datamodel_math",
    ], 
)