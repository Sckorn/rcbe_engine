cc_library(
    name = "rcbe_engine_camera",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_rendering",
    ],
)

### Packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")

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
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_rendering_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine camera libraries package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_camera",
    version = ENGINE_VERSION,
)

## Headers

pkg_tar(
    name = "camera-headers-data",
    srcs = [
        "include/camera/Camera.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/camera",
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
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_rendering_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_camera_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine camera headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_camera_dev",
    version = ENGINE_VERSION,
)
