load("//meta_package:global_variables.bzl", "PUBLIC_API_PREFIX")

cc_library(
    name = "rcbe_engine_camera",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    defines = PUBLIC_API_PREFIX + select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["RDMN_VULKAN"],
        "//meta_package:rdmn_gl_rasterizer_backend": ["RDMN_OPENGL"],
    }),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_fundamentals",
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_rendering",
        "//common:rdmn_trivial_logger",
    ],
)

### tests

cc_test(
    name = "rdmn_camera_tests",
    srcs = glob(
        ["test/*.cpp"],
    ),
    copts = ["-Iexternal/gtest/include"],
    data = [
        "//datamodel:datamodel_rendering_resources",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_engine_camera",
        "//common:engine_fuzzy_logic",
        "//common:engine_json_utils",
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_rendering",
        "//datamodel:engine_datamodel_visual",
        "@com_google_googletest//:gtest_main",
        "@nlohmann//:nlohmann_json",
    ],
)

### Packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## SO

pkg_tar(
    name = "camera-data",
    srcs = [
        ":rcbe_engine_camera",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "camera-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":camera-data",
    ],
)

pkg_deb(
    name = "camera-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":camera-package-data",
    depends = [
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_rendering_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine camera libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_camera",
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "camera-headers-data",
    srcs = [
        "include/camera/Camera.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/camera",
)

pkg_tar(
    name = "camera-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":camera-headers-data",
    ],
)

pkg_deb(
    name = "camera-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":camera-headers-package-data",
    depends = [
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_rendering_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_camera_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine camera headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_camera_dev",
    version = ENGINE_VERSION_FULL,
)
