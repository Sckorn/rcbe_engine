load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")

_PACKAGE_NAME = "camera"
_CAMERA_LIB_TARGET = "{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_CAMERA_LIB_TARGET = "{}_{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _CAMERA_LIB_TARGET,
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    defines = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["RDMN_VULKAN"],
        "//meta_package:rdmn_gl_rasterizer_backend": ["RDMN_OPENGL"],
    }),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:rdmn_engine_common_trivial_logger",
        "//common:rdmn_engine_common_fundamentals",
        "//datamodel:rdmn_engine_datamodel_math",
        "//datamodel:rdmn_engine_datamodel_rendering",
    ],
)

r_cc_shared_library(
    name = _SHARED_CAMERA_LIB_TARGET,
    deps = [_CAMERA_LIB_TARGET],
    shared_lib_name = _CAMERA_LIB_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = [
        "//common:rdmn_engine_common_trivial_logger_shared",
        "//datamodel:rdmn_engine_datamodel_math_shared",
        "//datamodel:rdmn_engine_datamodel_rendering_shared",
    ],
)

### tests

cc_test(
    name = "{}_tests".format(_CAMERA_LIB_TARGET),
    srcs = glob(
        ["test/*.cpp"],
    ),
    data = [
        "//datamodel:datamodel_rendering_resources",
    ],
    visibility = ["//visibility:public"],
    deps = [
        _CAMERA_LIB_TARGET,
        "//common:rdmn_engine_common_fuzzy_logic",
        "//common:rdmn_engine_common_json_utils",
        "//datamodel:rdmn_engine_datamodel_math",
        "//datamodel:rdmn_engine_datamodel_rendering",
        "//datamodel:rdmn_engine_datamodel_visual",
        "@com_google_googletest//:gtest_main",
        "@nlohmann//:nlohmann_json",
    ],
    dynamic_deps = [
        _SHARED_CAMERA_LIB_TARGET,
        "//common:rdmn_engine_common_json_utils_shared",
        "//datamodel:rdmn_engine_datamodel_math_shared",
        "//datamodel:rdmn_engine_datamodel_rendering_shared",
        "//datamodel:rdmn_engine_datamodel_visual_shared",
    ],
)

### Packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## SO

pkg_tar(
    name = "camera-data",
    srcs = [
        _SHARED_CAMERA_LIB_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
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
        "rdmn_engine_datamodel_math",
        "rdmn_engine_datamodel_rendering",
    ],
    description = "RDMN game engine camera libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _CAMERA_LIB_TARGET,
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "camera-headers-data",
    srcs = [
        "include/camera/Camera.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/camera",
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
        "rdmn_engine_datamodel_math_dev",
        "rdmn_engine_datamodel_rendering_dev",
        _CAMERA_LIB_TARGET,
    ],
    description = "RDMN game engine camera headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_CAMERA_LIB_TARGET),
    version = ENGINE_VERSION_FULL,
)
