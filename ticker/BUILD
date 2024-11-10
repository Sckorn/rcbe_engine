load("//meta_package:global_variables.bzl", "PUBLIC_API_PREFIX")

cc_library(
    name = "rcbe_ticker",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    defines = PUBLIC_API_PREFIX,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//traits:rcbe_traits",
        "//common:rdmn_trivial_logger",
    ],
)

### tests

cc_test(
    name = "rcbe_ticker_tests",
    srcs = glob(["test/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    defines = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["RDMN_VULKAN"],
        "//meta_package:rdmn_gl_rasterizer_backend": [
            "RDMN_OPENGL",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_ticker",
        "@com_google_googletest//:gtest_main",
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
    name = "ticker-data",
    srcs = [
        ":rcbe_ticker",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
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
        "libboost-log{}.0".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine ticker libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_ticker",
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "ticker-headers-data",
    srcs = [
        "include/ticker/Ticker.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/ticker",
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
        "rcbe_type_traits_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_ticker_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine ticker headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_ticker_dev",
    version = ENGINE_VERSION_FULL,
)
