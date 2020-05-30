cc_library(
    name = "x3d_parser",
    srcs = ["src/x3d/X3dParser.cpp"],
    hdrs = ["include/parsers/x3d/X3dParser.hpp"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_filesystem_utils",
        "//datamodel:engine_datamodel_geometry",
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

# packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta-package:global_variables.bzl", "ENGINE_VERSION")

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
    architecture = "amd64",
    built_using = "unzip (6.0.1)",
    data = ":parsers-package-data",
    depends = [
        "rcbe_common",
        "rcbe_datamodel",
    ],
    description = "RCBE game engine parser libraries package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_parsers",
    version = ENGINE_VERSION,
)
