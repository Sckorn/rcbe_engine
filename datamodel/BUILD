cc_library(
    name = "engine_datamodel_core",
    srcs = glob([
        "src/core/**/*.cpp",
    ]),
    hdrs = glob([
        "include/data_types/config/data_model_config.hpp",
        "include/data_types/core/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_fundamentals",
        "@nlohmann",
    ],
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
        "//common:engine_math_utils",
        "@nlohmann",
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
    name = "engine_datamodel_system",
    srcs = glob([
        "src/system/**/*.cpp",
    ]),
    hdrs = glob([
        "include/data_types/system/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        ":engine_datamodel_math",
        ":engine_datamodel_visual",
        "@boost//:bimap",
        "@nlohmann",
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
    copts = ["-Iexternal/nlohmann/include"],
    includes = ["include"],
    linkopts = [
        "-lpthread",
        "-lOpenGL",
        "-lGL",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        ":engine_datamodel_geometry",
        ":engine_datamodel_math",
        ":engine_datamodel_visual",
        "//gl_extensions",
        "@boost//:log",
        "@nlohmann",
    ],
)

filegroup(
    name = "datamodel_rendering_resources",
    srcs = glob(["data/rendering/*"]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "datamodel_system_resources",
    srcs = glob(["data/system/*"]),
    visibility = ["//visibility:public"],
)

# tests

cc_test(
    name = "engine_datamodel_test",
    srcs = glob(["test/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    data = [":datamodel_rendering_resources"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_math",
        ":engine_datamodel_rendering",
        ":engine_datamodel_visual",
        "//common:engine_fuzzy_logic",
        "//common:engine_json_utils",
        "@gtest",
        "@nlohmann",
    ],
)

# packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")

pkg_tar(
    name = "datamodel-data",
    srcs = [
        ":engine_datamodel_core",
        ":engine_datamodel_geometry",
        ":engine_datamodel_math",
        ":engine_datamodel_rendering",
        ":engine_datamodel_system",
        ":engine_datamodel_visual",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "datamodel-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-data",
    ],
)

pkg_deb(
    name = "datamodel-deb",
    architecture = "amd64",
    built_using = "unzip (6.0.1)",
    data = ":datamodel-package-data",
    depends = ["rcbe_common"],
    description = "RCBE game engine datamodel libraries package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_datamodel",
    version = ENGINE_VERSION,
)
