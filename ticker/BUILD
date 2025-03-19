load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")

_PACKAGE_NAME = "ticker"

_TICKER_TARGET = "{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_TICKER_TARGET = "{}_{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _TICKER_TARGET,
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:rdmn_engine_common_trivial_logger",
        "//traits:rdmn_engine_type_traits",
    ],
)

r_cc_shared_library(
    name = _SHARED_TICKER_TARGET,
    deps = [_TICKER_TARGET],
    shared_lib_name = _TICKER_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = [
        "//common:rdmn_engine_common_trivial_logger_shared",
    ],
)

### tests

cc_test(
    name = "{}_tests".format(_TICKER_TARGET),
    srcs = glob(["test/*.cpp"]),
    defines = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["RDMN_VULKAN"],
        "//meta_package:rdmn_gl_rasterizer_backend": [
            "RDMN_OPENGL",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        _TICKER_TARGET,
        "@com_google_googletest//:gtest_main",
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
    name = "ticker-data",
    srcs = [
        _SHARED_TICKER_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "ticker-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":ticker-data",
    ],
)

pkg_deb(
    name = "ticker-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":ticker-package-data",
    depends = [
        "rdmn_engine_common_trivial_logger",
    ],
    description = "RDMN game engine ticker libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _TICKER_TARGET,
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "ticker-headers-data",
    srcs = [
        "include/ticker/Ticker.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/ticker",
)

pkg_tar(
    name = "ticker-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":ticker-headers-data",
    ],
)

pkg_deb(
    name = "ticker-header-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":ticker-headers-package-data",
    depends = [
        "rdmn_engine_type_traits_dev",
        _TICKER_TARGET,
    ],
    description = "RDMN game engine ticker headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_TICKER_TARGET),
    version = ENGINE_VERSION_FULL,
)
