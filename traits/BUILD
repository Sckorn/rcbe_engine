load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")

_PACKAGE_NAME = "type_traits"

_TRAITS_TARGET = "{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)

r_cc_library(
    name = _TRAITS_TARGET,
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)

### Packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## Headers

pkg_tar(
    name = "type-traits-headers-data",
    srcs = glob([
        "include/traits/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/traits",
)

pkg_tar(
    name = "type-traits-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":type-traits-headers-data",
    ],
)

pkg_deb(
    name = "type-traits-header-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":type-traits-headers-package-data",
    description = "RDMN game engine type traits headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_TRAITS_TARGET),
    version = ENGINE_VERSION_FULL,
)
