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

# packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")

pkg_tar(
    name = "window-manager-data",
    srcs = [
        ":rcbe_window_manager",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "wm-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":window-manager-data",
    ],
)

pkg_deb(
    name = "window-manager-deb",
    architecture = "amd64",
    built_using = "unzip (6.0.1)",
    data = ":wm-package-data",
    depends = ["rcbe_datamodel"],
    description = "RCBE game engine window manager libraries package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_window_manager",
    version = ENGINE_VERSION,
)
