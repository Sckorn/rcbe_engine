load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")

_PARSER_INCLUDE_PATH = "include"

_PACKAGE_NAME = "parser"

_X3D_TARGET = "{}_{}_x3d".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_X3D_TARGET = "{}_{}_x3d_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _X3D_TARGET,
    srcs = ["src/x3d/x3d_parser.cpp"],
    hdrs = ["include/parsers/x3d/x3d_parser.hpp"],
    include_prefix = "rcbe-engine",
    includes = [_PARSER_INCLUDE_PATH],
    strip_include_prefix = _PARSER_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        "//common:rdmn_engine_common_filesystem_utils",
        "//datamodel:rdmn_engine_datamodel_geometry",
        "//datamodel:rdmn_engine_datamodel_math",
        "@tinyxml2//:tinyxml2",
    ],
)

r_cc_shared_library(
    name = _SHARED_X3D_TARGET,
    deps = [_X3D_TARGET,],
    shared_lib_name = _X3D_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = [
        "//common:rdmn_engine_common_filesystem_utils_shared",
        "//datamodel:rdmn_engine_datamodel_geometry_shared",
        "//datamodel:rdmn_engine_datamodel_visual_shared",
        "//datamodel:rdmn_engine_datamodel_math_shared",
    ],
)

_STL_TARGET = "{}_{}_stl".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_STL_TARGET = "{}_{}_stl_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _STL_TARGET,
    srcs = ["src/stl/stl_parser.cpp"],
    hdrs = ["include/parsers/stl/stl_parser.hpp"],
    include_prefix = "rcbe-engine",
    includes = [_PARSER_INCLUDE_PATH],
    strip_include_prefix = _PARSER_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:rdmn_engine_datamodel_geometry",
        "//datamodel:rdmn_engine_datamodel_math",
    ],
)

r_cc_shared_library(
    name = _SHARED_STL_TARGET,
    deps = [_STL_TARGET],
    shared_lib_name = _STL_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = [
        "//datamodel:rdmn_engine_datamodel_geometry_shared",
        "//datamodel:rdmn_engine_datamodel_math_shared",
        "//datamodel:rdmn_engine_datamodel_visual_shared",
        "//common:rdmn_engine_common_binary_tools_shared",
    ],
)

_TGA_TARGET = "{}_{}_tga".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_TGA_TARGET = "{}_{}_tga_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _TGA_TARGET,
    srcs = ["src/tga/tga_parser.cpp"],
    hdrs = ["include/parsers/tga/tga_parser.hpp"],
    include_prefix = "rcbe-engine",
    includes = [_PARSER_INCLUDE_PATH],
    strip_include_prefix = _PARSER_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        "//datamodel:rdmn_engine_datamodel_math",
        "//datamodel:rdmn_engine_datamodel_visual",
        "//common:rdmn_engine_common_trivial_logger",
    ],
)

r_cc_shared_library(
    name = _SHARED_TGA_TARGET,
    deps = [_TGA_TARGET],
    shared_lib_name = _TGA_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = [
        "//datamodel:rdmn_engine_datamodel_math_shared",
        "//datamodel:rdmn_engine_datamodel_visual_shared",
        "//common:rdmn_engine_common_trivial_logger_shared",
        "//common:rdmn_engine_common_binary_tools_shared",
    ],
)


_GLTF_TARGET = "{}_{}_gltf".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_GLTF_TARGET = "{}_{}_gltf_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _GLTF_TARGET,
    srcs = ["src/gltf/gltf_parser.cpp"],
    hdrs = ["include/parsers/gltf/gltf_parser.hpp"],
    data = [
        "//datamodel:datamodel_rendering_resources",
    ],
    defines = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["RDMN_VULKAN"],
        "//meta_package:rdmn_gl_rasterizer_backend": ["RDMN_OPENGL"],
    }),
    include_prefix = "rcbe-engine",
    includes = [_PARSER_INCLUDE_PATH],
    linkopts = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["-lvulkan"],
        "//meta_package:rdmn_gl_rasterizer_backend": ["-lGL"],
    }),
    strip_include_prefix = _PARSER_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        _TGA_TARGET,
        "//common:rdmn_engine_common_fundamentals",
        "//common:rdmn_engine_common_json_utils",
        "//common:rdmn_engine_common_binary_tools",
        "//common:rdmn_engine_common_trivial_logger",
        "//datamodel:rdmn_engine_datamodel_core",
        "//datamodel:rdmn_engine_datamodel_geometry",
        "//datamodel:rdmn_engine_datamodel_math",
        "//datamodel:rdmn_engine_datamodel_rendering",
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_GLTF_TARGET,
    deps = [_GLTF_TARGET],
    shared_lib_name = _GLTF_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = [
        _SHARED_TGA_TARGET,
        "//common:rdmn_engine_common_json_utils_shared",
        "//common:rdmn_engine_common_binary_tools_shared",
        "//common:rdmn_engine_common_trivial_logger_shared",
        "//datamodel:rdmn_engine_datamodel_core_shared",
        "//datamodel:rdmn_engine_datamodel_geometry_shared",
        "//datamodel:rdmn_engine_datamodel_math_shared",
        "//datamodel:rdmn_engine_datamodel_rendering_shared",
        "//datamodel:rdmn_engine_datamodel_visual_shared",
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
    name = "{}_tests".format(_X3D_TARGET),
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
        _X3D_TARGET,
        "//datamodel:rdmn_engine_datamodel_math",
        "@com_google_googletest//:gtest_main",
    ],
    dynamic_deps = [
        _SHARED_X3D_TARGET,
        "//datamodel:rdmn_engine_datamodel_geometry_shared",
    ],
)

cc_test(
    name = "{}_tests".format(_STL_TARGET),
    srcs = [
        "test/parsers_test.cpp",
        "test/stl_parser_test.cpp",
    ],
    data = [
        "@low_poly_wolf_stl//file",
        "@stl_test_data_archive//:stl_tests_archive",
    ],
    visibility = ["//visibility:public"],
    deps = [
        _STL_TARGET,
        "//datamodel:rdmn_engine_datamodel_math",
        "@com_google_googletest//:gtest_main",
    ],
    dynamic_deps = [
        _SHARED_STL_TARGET,
        "//datamodel:rdmn_engine_datamodel_geometry_shared",
    ],
)

cc_test(
    name = "{}_tests".format(_TGA_TARGET),
    srcs = [
        "test/parsers_test.cpp",
        "test/tga_parser_test.cpp",
    ],
    data = [
        "//datamodel:datamodel_rendering_resources",
        "@brick_wall_texture//file",
    ],
    visibility = ["//visibility:public"],
    deps = [
        _TGA_TARGET,
        "//datamodel:rdmn_engine_datamodel_math",
        "@com_google_googletest//:gtest_main",
    ],
    dynamic_deps = [
        _SHARED_TGA_TARGET,
    ],
)

cc_test(
    name = "{}_tests".format(_GLTF_TARGET),
    srcs = [
        "test/parsers_test.cpp",
        "test/gltf_parser_test.cpp",
    ],
    data = [
        "//datamodel:datamodel_rendering_resources",
        "@gltf_test_data_archive//:gltf_tests_archive",
    ],
    visibility = ["//visibility:public"],
    deps = [
        _GLTF_TARGET,
        "//datamodel:rdmn_engine_datamodel_math",
        "@com_google_googletest//:gtest_main",
    ],
    dynamic_deps = [
        _SHARED_GLTF_TARGET,
        "//datamodel:rdmn_engine_datamodel_geometry_shared",
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## SO

# X3D

pkg_tar(
    name = "x3d-parser-data",
    srcs = [
        _SHARED_X3D_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "x3d-parser-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":x3d-parser-data",
    ],
)

pkg_deb(
    name = "parsers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":x3d-parser-package-data",
    depends = [
        "rdmn_engine_common_filesystem_utils",
        "rdmn_engine_datamodel_geometry",
        "rdmn_engine_datamodel_math",
        "libtinyxml2-10",
    ],
    description = "RDMN game engine parser libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _X3D_TARGET,
    version = ENGINE_VERSION_FULL,
)

# STL

pkg_tar(
    name = "stl-parsers-data",
    srcs = [
        _SHARED_STL_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
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
        "rdmn_engine_common_filesystem_utils",
        "rdmn_engine_datamodel_geometry",
        "rdmn_engine_datamodel_math",
        "rdmn_engine_common_binary_tools",
    ],
    description = "RDMN game engine parser libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _STL_TARGET,
    version = ENGINE_VERSION_FULL,
)

# TGA

pkg_tar(
    name = "tga-parsers-data",
    srcs = [
        _SHARED_TGA_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "tga-parsers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":tga-parsers-data",
    ],
)

pkg_deb(
    name = "tga-parsers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":tga-parsers-package-data",
    depends = [
        "rdmn_engine_common_filesystem_utils",
        "rdmn_engine_datamodel_geometry",
        "rdmn_engine_datamodel_math",
        "rdmn_engine_common_binary_tools",
    ],
    description = "RDMN game engine TGA images parser lib package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _TGA_TARGET,
    version = ENGINE_VERSION_FULL,
)

# GLTF

pkg_tar(
    name = "gltf-parsers-data",
    srcs = [
        _SHARED_GLTF_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "gltf-parsers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":gltf-parsers-data",
    ],
)

pkg_deb(
    name = "gltf-parsers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":gltf-parsers-package-data",
    depends = [
        "rdmn_engine_common_filesystem_utils",
        "rdmn_engine_common_binary_tools",
        "rdmn_engine_datamodel_geometry",
        "rdmn_engine_datamodel_math",
    ],
    description = "RDMN game engine GLTF models parser lib package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _GLTF_TARGET,
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
    package_dir = "/usr/include/rcbe/rcbe-engine/parsers/x3d",
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
        "rcbe_engine_common_filesystem_utils_dev",
        "rcbe_engine_datamodel_geometry_dev",
        "rcbe_engine_datamodel_math_dev",
        _X3D_TARGET,
        "libtinyxml2-dev",
    ],
    description = "RDMN game engine parser headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_X3D_TARGET),
    version = ENGINE_VERSION_FULL,
)

# STL

pkg_tar(
    name = "parsers-stl-headers-data-dev",
    srcs = [
        "include/parsers/stl/stl_parser.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/parsers/stl",
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
        "rdmn_engine_common_filesystem_utils_dev",
        "rdmn_engine_datamodel_geometry_dev",
        "rdmn_engine_datamodel_math_dev",
        _STL_TARGET,
    ],
    description = "RDMN game engine STL parser headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_STL_TARGET),
    version = ENGINE_VERSION_FULL,
)

# TGA

pkg_tar(
    name = "parsers-tga-headers-data-dev",
    srcs = ["include/parsers/tga/tga_parser.hpp"],
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/parsers/tga",
)

pkg_tar(
    name = "parsers-tga-headers-package-data-dev",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":parsers-tga-headers-data-dev",
    ],
)

pkg_deb(
    name = "parsers-tga-headers-deb-dev",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":parsers-tga-headers-package-data-dev",
    depends = [
        "rdmn_engine_common_filesystem_utils_dev",
        "rdmn_engine_datamodel_geometry_dev",
        "rdmn_engine_datamodel_math_dev",
        _TGA_TARGET,
    ],
    description = "RDMN game engine TGA images parser headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_TGA_TARGET),
    version = ENGINE_VERSION_FULL,
)

# GLTF

pkg_tar(
    name = "parsers-gltf-headers-data-dev",
    srcs = ["include/parsers/gltf/gltf_parser.hpp"],
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/parsers/gltf",
)

pkg_tar(
    name = "parsers-gltf-headers-package-data-dev",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":parsers-gltf-headers-data-dev",
    ],
)

pkg_deb(
    name = "parsers-gltf-headers-deb-dev",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":parsers-gltf-headers-package-data-dev",
    depends = [
        "rdmn_engine_common_filesystem_utils_dev",
        "rdmn_engine_datamodel_geometry_dev",
        "rdmn_engine_datamodel_math_dev",
        _GLTF_TARGET,
    ],
    description = "RDMN game engine GLTF models parser headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_GLTF_TARGET),
    version = ENGINE_VERSION_FULL,
)
