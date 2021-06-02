cc_library(
    name = "rcbe_window_manager",
    srcs = glob(
        [
            "src/**/*.cpp",
            "src/**/*.hpp",
        ],
        exclude = ["src/main.cpp"],
    ),
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    linkopts = [
        "-lpthread",
        "-lX11",
        "-lXext",
    ],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_json_utils",
        "//datamodel:engine_datamodel_core",
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_system",
        "//datamodel:engine_datamodel_visual",
        "//input_manager:rcbe_input_manager",
        "//renderer:rcbe_renderer",
        "@boost//:log",
        "@nlohmann",
    ],
)

cc_binary(
    name = "check_tga",
    srcs = ["src/main.cpp"],
    data = [
        "@awesomeface_texture//file",
    ],
    linkopts = [
        "-lpthread",
        "-lX11",
        "-lXext",
    ],
    tags = ["local"],
    deps = [
        ":rcbe_window_manager",
        "//common:engine_output_utils",
        "//datamodel:engine_datamodel_visual",
        "@boost//:log",
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## SO

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
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":wm-package-data",
    depends = [
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_system_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_input_manager_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_renderer_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-log{}.0".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine window manager libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_window_manager",
    version = ENGINE_VERSION,
)

## Headers

pkg_tar(
    name = "window-manager-headers-data",
    srcs = glob([
        "include/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/core",
)

pkg_tar(
    name = "wm-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":window-manager-headers-data",
    ],
)

pkg_deb(
    name = "window-manager-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":wm-headers-package-data",
    depends = [
        "rcbe_datamodel_core_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_system_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_input_manager_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_renderer_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_window_manager_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-log{}-dev".format(BOOST_TARGET_VERSION),
        "nlohmann-json3-dev",
    ],
    description = "RCBE game engine window manager headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_window_manager_dev",
    version = ENGINE_VERSION,
)
