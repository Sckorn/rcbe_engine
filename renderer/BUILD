cc_library(
    name = "rcbe_renderer",
    srcs = glob(
        ["src/**/*.cpp"],
        exclude = [
            "src/main.cpp",
        ],
    ),
    hdrs = glob(["include/renderer/**/*.hpp"]),
    copts = [
        "-DRCBE_GLM_INTEGRATION",
        "-DRCBE_DEBUG_MODE",
    ],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    linkopts = [
        "-lpthread",
        "-lGL",
        "-lGLX",
        "-lX11",
    ],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//camera:rcbe_engine_camera",
        "//datamodel:engine_datamodel_core",
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_rendering",
        "//datamodel:engine_datamodel_visual",
        "//gl_extensions",
        "//ticker:rcbe_ticker",
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
        "@awesomeface_texture//file",
        "@brick_wall_texture//file",
        "@low_poly_wolf_stl//file",
    ],
    linkopts = [
        "-lpthread",
        "-lX11",
        "-lXext",
    ],
    tags = ["local"],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_renderer",
        "//camera:rcbe_engine_camera",
        "//common:engine_json_utils",
        "//common:engine_output_utils",
        "//common:rcbe_profiling",
        "//datamodel:engine_datamodel_math",
        "//parsers:rcbe_stl_parser",
        "//parsers:x3d_parser",
        "//window_manager:rcbe_window_manager",
        "@boost//:log",
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## SO

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
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":renderer-package-data",
    depends = [
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_rendering_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_visual_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_window_manager_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_gl_extensions_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_ticker_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "libboost-log{}.0".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine renderer libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_renderer",
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "renderer-headers-data",
    srcs = [
        "include/renderer/GLRenderer.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/renderer",
)

pkg_tar(
    name = "renderer-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":renderer-headers-data",
    ],
)

pkg_deb(
    name = "renderer-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":renderer-headers-package-data",
    depends = [
        "rcbe_datamodel_core_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_rendering_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_visual_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_gl_extensions_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_window_manager_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_ticker_dev{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_renderer_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "libboost-log{}-dev".format(BOOST_TARGET_VERSION),
        "nlohmann-json3-dev",
    ],
    description = "RCBE game engine renderer headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_renderer_dev",
    version = ENGINE_VERSION_FULL,
)
