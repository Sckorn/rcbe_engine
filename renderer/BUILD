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
        "//window_manager:rcbe_window_manager",
    ],
)

# packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")

pkg_tar(
    name = "renderer-data",
    srcs = [
        ":rcbe_renderer",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "renderer-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":renderer-data",
    ],
)

pkg_deb(
    name = "renderer-deb",
    architecture = "amd64",
    built_using = "unzip (6.0.1)",
    data = ":renderer-package-data",
    depends = [
        "rcbe_common",
        "rcbe_datamodel",
        "rcbe_window_manager",
        "rcbe_gl_extensions",
    ],
    description = "RCBE game engine renderer libraries package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_renderer",
    version = ENGINE_VERSION,
)
