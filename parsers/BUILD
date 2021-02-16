cc_library(
    name = "x3d_parser",
    srcs = ["src/x3d/x3d_parser.cpp"],
    hdrs = ["include/parsers/x3d/x3d_parser.hpp"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_filesystem_utils",
        "//datamodel:engine_datamodel_geometry",
        "//datamodel:engine_datamodel_math",
        "@boost//:property_tree",
    ],
)

cc_library(
    name = "rcbe_stl_parser",
    srcs = ["src/stl/stl_parser.cpp"],
    hdrs = ["include/parsers/stl/stl_parser.hpp"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:engine_datamodel_geometry",
        "//datamodel:engine_datamodel_math",
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
    name = "rcbe_x3d_parsers_tests",
    srcs = [
        "test/parsers_test.cpp",
        "test/x3d_parser_test.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    data = [
        "test/resources/corner.x3d",
        "test/resources/simple_edge.x3d",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":x3d_parser",
        "//datamodel:engine_datamodel_math",
        "@gtest",
    ],
)

cc_test(
    name = "rcbe_stl_parsers_tests",
    srcs = [
        "test/parsers_test.cpp",
        "test/stl_parser_test.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    data = [
        "@low_poly_wolf_stl//file",
        "@stl_test_data_archive//:stl_tests_archive",
    ],
    tags = ["local"],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_stl_parser",
        "//datamodel:engine_datamodel_math",
        "@gtest",
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## SO

# X3D

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
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_parsers",
    version = ENGINE_VERSION,
)

# STL

pkg_tar(
    name = "stl-parsers-data",
    srcs = [
        ":rcbe_stl_parser",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "stl-parsers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":stl-parsers-data",
    ],
)

pkg_deb(
    name = "stl-parsers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":stl-parsers-package-data",
    depends = [
        "rcbe_common_filesystem_utils_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_geometry_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_common_binary_tools_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine parser libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_stl_parsers",
    version = ENGINE_VERSION,
)

## Headers

# X3D

pkg_tar(
    name = "parsers-headers-data",
    srcs = [
        "include/parsers/x3d/x3d_parser.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/parsers/x3d",
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
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_parsers_dev",
    version = ENGINE_VERSION,
)

# STL

pkg_tar(
    name = "parsers-stl-headers-data-dev",
    srcs = [
        "include/parsers/stl/stl_parser.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/parsers/stl",
)

pkg_tar(
    name = "parsers-stl-headers-package-data-dev",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":parsers-stl-headers-data-dev",
    ],
)

pkg_deb(
    name = "parsers-stl-headers-deb-dev",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":parsers-stl-headers-package-data-dev",
    depends = [
        "rcbe_common_filesystem_utils_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_geometry_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_stl_parsers_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine STL parser headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_stl_parsers_dev",
    version = ENGINE_VERSION,
)
