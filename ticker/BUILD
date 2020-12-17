cc_library(
    name = "rcbe_ticker",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "//traits:rcbe_traits",
        "@boost//:log",
    ],
)

### tests

cc_test(
    name = "rcbe_ticker_tests",
    srcs = glob(["test/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_ticker",
        "@gtest",
    ],
)

### Packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
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
    version = ENGINE_VERSION,
)

## Headers

pkg_tar(
    name = "ticker-headers-data",
    srcs = [
        "include/ticker/Ticker.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/ticker",
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
        "rcbe_type_traits_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_ticker_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine ticker headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_ticker_dev",
    version = ENGINE_VERSION,
)
