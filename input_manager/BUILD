cc_library(
    name = "rcbe_input_manager",
    srcs = glob(
        ["src/**/*.cpp"],
        exclude = ["src/main.cpp"],
    ),
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    linkopts = [
        "-lpthread",
        "-lOpenGL",
        "-lGL",
        "-lGLX",
        "-lX11",
    ],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:rcbe_delegate",
        "//datamodel:engine_datamodel_system",
        "//gl_extensions",
        "//traits:rcbe_traits",
        "@boost//:bimap",
        "@boost//:log",
    ],
)

### Packaging

## SO

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

pkg_tar(
    name = "input-manager-data",
    srcs = [
        ":rcbe_input_manager",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "input-manager-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":input-manager-data",
    ],
)

pkg_deb(
    name = "input-manager-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":input-manager-package-data",
    depends = [
        "rcbe_datamodel_system_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-log{}.0".format(BOOST_TARGET_VERSION),
        "libboost-container{}.0".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine input manager libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_input_manager",
    version = ENGINE_VERSION,
)

## Headers

pkg_tar(
    name = "input-manager-headers-data",
    srcs = glob([
        "include/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/core/",
)

pkg_tar(
    name = "input-manager-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":input-manager-headers-data",
    ],
)

pkg_deb(
    name = "input-manager-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":input-manager-headers-package-data",
    depends = ["rcbe_input_manager_{}_{}".format(ENGINE_VERSION, TARGET_ARCH)],
    description = "RCBE game engine input manager headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_input_manager_dev",
    version = ENGINE_VERSION,
)
