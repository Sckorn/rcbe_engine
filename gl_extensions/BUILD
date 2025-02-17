load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")

_PACKAGE_NAME = "gl_extensions"

_GL_EXT_TARGET = "{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_GL_EXT_TARGET = "{}_{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _GL_EXT_TARGET,
    srcs = glob(
        ["src/gl_extensions.cpp"],
    ),
    hdrs = glob(["include/core/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)

r_cc_shared_library(
    name = _SHARED_GL_EXT_TARGET,
    deps = [_GL_EXT_TARGET],
    shared_lib_name = _GL_EXT_TARGET,
    visibility = ["//visibility:public"],
)

### packaging

## SO

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

pkg_tar(
    name = "gl-extensions-data",
    srcs = [
        _SHARED_GL_EXT_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "gl-extensions-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":gl-extensions-data",
    ],
)

pkg_deb(
    name = "gl-extensions-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":gl-extensions-package-data",
    description = "RDMN game engine gl extensions libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _GL_EXT_TARGET,
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "gl-extensions-headers-data",
    srcs = glob([
        "include/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/core/",
)

pkg_tar(
    name = "gl-extensions-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":gl-extensions-headers-data",
    ],
)

pkg_deb(
    name = "gl-extensions-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":gl-extensions-headers-package-data",
    depends = [_GL_EXT_TARGET],
    description = "RDMN game engine gl extensions headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_GL_EXT_TARGET),
    version = ENGINE_VERSION_FULL,
)
