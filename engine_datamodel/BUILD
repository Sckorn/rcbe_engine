cc_library(
    name = "engine_datamodel_core",
    hdrs = glob([
        "include/data_types/config/data_model_config.hpp",
        "include/data_types/core/**/*.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = ["//engine_common:engine_fundamentals"],
)

cc_library(
    name = "engine_datamodel_math",
    srcs = glob([
        "src/math/**/*.cpp",
    ]),
    hdrs = glob([
        "include/data_types/math/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        "@nlohmann",
        "//engine_common:engine_math_utils",
    ],
)

cc_library(
    name = "engine_datamodel_visual",
    srcs = glob([
        "src/visual/**/*.cpp",
    ]),
    hdrs = glob([
        "include/data_types/visual/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_math",
        "@nlohmann",
    ],
)

cc_library(
    name = "engine_datamodel_geometry",
    srcs = glob(["src/geometry/**/*.cpp"]),
    hdrs = glob([
        "include/data_types/geometry/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        ":engine_datamodel_visual",
        "@nlohmann",
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
    srcs = glob([
        "src/rendering/**/*.cpp",
    ]),
    hdrs = glob([
        "include/data_types/rendering/**/*.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        ":engine_datamodel_geometry",
        ":engine_datamodel_math",
    ],
)

# tests

cc_test(
    name = "engine_datamodel_test",
    srcs = glob(["test/*.cpp"]),
    deps = [
        ":engine_datamodel_math",
        "//engine_common:engine_fuzzy_logic",
        "@gtest",
    ],
    copts = ["-Iexternal/gtest/include"],
    visibility = ["//visibility:public"],
    #data = glob(["test/resources/*"]),
)