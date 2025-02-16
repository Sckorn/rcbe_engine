load("//meta_package:global_variables.bzl", "PUBLIC_API_PREFIX")

cc_library(
    name = "x3d_parser",
    srcs = ["src/x3d/x3d_parser.cpp"],
    hdrs = ["include/parsers/x3d/x3d_parser.hpp"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    defines = PUBLIC_API_PREFIX,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "@tinyxml2//:tinyxml2",
        "//common:engine_filesystem_utils",
        "//datamodel:engine_datamodel_geometry",
        "//datamodel:engine_datamodel_math",
    ],
)

cc_library(
    name = "rcbe_stl_parser",
    srcs = ["src/stl/stl_parser.cpp"],
    hdrs = ["include/parsers/stl/stl_parser.hpp"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    defines = PUBLIC_API_PREFIX,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:engine_datamodel_geometry",
        "//datamodel:engine_datamodel_math",
    ],
)

cc_library(
    name = "rdmn_tga_parser",
    srcs = ["src/tga/tga_parser.cpp"],
    hdrs = ["include/parsers/tga/tga_parser.hpp"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    defines = PUBLIC_API_PREFIX,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_visual",
        "//meta_package/public_api:rdmn_public_api",
        "//common:rdmn_trivial_logger",
    ],
)

cc_library(
    name = "rdmn_gltf_parser",
    srcs = ["src/gltf/gltf_parser.cpp"],
    hdrs = ["include/parsers/gltf/gltf_parser.hpp"],
    copts = ["-Iexternal/nlohmann/include"],
    data = [
        "//datamodel:datamodel_rendering_resources",
    ],
    defines = PUBLIC_API_PREFIX + select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["RDMN_VULKAN"],
        "//meta_package:rdmn_gl_rasterizer_backend": ["RDMN_OPENGL"],
    }),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    linkopts = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["-lvulkan"],
        "//meta_package:rdmn_gl_rasterizer_backend": ["-lGL"],
    }),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        ":rdmn_tga_parser",
        "//common:engine_fundamentals",
        "//common:engine_json_utils",
        "//common:rcbe_binary_tools",
        "//datamodel:engine_datamodel_core",
        "//datamodel:engine_datamodel_geometry",
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_rendering",
        "//common:rdmn_trivial_logger",
        "@nlohmann//:nlohmann_json",
    ],
)

### Remove after debug

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
    data = [
        "test/resources/corner.x3d",
        "test/resources/simple_edge.x3d",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":x3d_parser",
        "//datamodel:engine_datamodel_math",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "rcbe_stl_parsers_tests",
    srcs = [
        "test/parsers_test.cpp",
        "test/stl_parser_test.cpp",
    ],
    data = [
        "@low_poly_wolf_stl//file",
        "@stl_test_data_archive//:stl_tests_archive",
    ],
    tags = ["local"],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_stl_parser",
        "//datamodel:engine_datamodel_math",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "rdmn_tga_parsers_tests",
    srcs = [
        "test/parsers_test.cpp",
        "test/tga_parser_test.cpp",
    ],
    data = [
        "//datamodel:datamodel_rendering_resources",
        "@brick_wall_texture//file",
    ],
    tags = ["local"],
    visibility = ["//visibility:public"],
    deps = [
        ":rdmn_tga_parser",
        "//datamodel:engine_datamodel_math",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "rdmn_gltf_parsers_test",
    srcs = [
        "test/parsers_test.cpp",
        "test/gltf_parser_test.cpp",
    ],
    data = [
        "//datamodel:datamodel_rendering_resources",
        "@gltf_test_data_archive//:gltf_tests_archive",
    ],
    tags = ["local"],
    visibility = ["//visibility:public"],
    deps = [
        ":rdmn_gltf_parser",
        "//datamodel:engine_datamodel_math",
        "@com_google_googletest//:gtest_main",
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
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
        "rcbe_common_filesystem_utils_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_geometry_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine parser libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_parsers",
    version = ENGINE_VERSION_FULL,
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
        "rcbe_common_filesystem_utils_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_geometry_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_common_binary_tools_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine parser libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_stl_parsers",
    version = ENGINE_VERSION_FULL,
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
        "rcbe_common_filesystem_utils_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_geometry_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_parsers_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine parser headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_parsers_dev",
    version = ENGINE_VERSION_FULL,
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
        "rcbe_common_filesystem_utils_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_geometry_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_stl_parsers_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine STL parser headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_stl_parsers_dev",
    version = ENGINE_VERSION_FULL,
)
