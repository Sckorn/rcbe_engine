cc_library(
    name = "x3d_parser",
    srcs = ["src/x3d/X3dParser.cpp"],
    hdrs = ["include/parsers/x3d/X3dParser.hpp"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_filesystem_utils",
        "//datamodel:engine_datamodel_geometry",
        "//datamodel:engine_datamodel_math",
        "@boost//:property_tree",
    ],
)

filegroup(
    name = "x3d_test_resources",
    srcs = glob([
        "test/resources/*",
    ]),
    visibility = ["//visibility:public"],
)

### tests

cc_test(
    name = "x3d_parser_test",
    srcs = glob(["test/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    data = glob(["test/resources/*"]),
    visibility = ["//visibility:public"],
    deps = [
        ":x3d_parser",
        "//datamodel:engine_datamodel_math",
        "@gtest",
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")

## SO

pkg_tar(
    name = "parsers-data",
    srcs = [
        ":x3d_parser",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "parsers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":parsers-data",
    ],
)

pkg_deb(
    name = "parsers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":parsers-package-data",
    depends = [
        "rcbe_common_filesystem_utils_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_geometry_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine parser libraries package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_parsers",
    version = ENGINE_VERSION,
)

## Headers

pkg_tar(
    name = "parsers-headers-data",
    srcs = [
        "include/parsers/x3d/X3dParser.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/parsers/x3d",
)

pkg_tar(
    name = "parsers-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":parsers-headers-data",
    ],
)

pkg_deb(
    name = "parsers-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":parsers-headers-package-data",
    depends = [
        "rcbe_common_filesystem_utils_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_geometry_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_parsers_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine parser headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_parsers_dev",
    version = ENGINE_VERSION,
)
