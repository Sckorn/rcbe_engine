load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")

_PACKAGE_NAME = "renderer"

_RENDERER_TARGET = "{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_RENDERER_TARGET = "{}_{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _RENDERER_TARGET,
    srcs = select({
        "//meta_package:rdmn_vk_rasterizer_backend": [
            "src/VulkanRenderer.cpp",
            "src/VulkanRenderer.hpp",
        ],
        "//meta_package:rdmn_gl_rasterizer_backend": [
            "src/GLRenderer.cpp",
            "src/GLRenderer.hpp",
        ],
    }) + [
        "src/Renderer.cpp",
    ],
    hdrs = ["include/renderer/Renderer.hpp"],
    data = [
        "//datamodel:datamodel_rendering_resources",
        "//parsers:x3d_test_resources",
    ],
    defines = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["RDMN_VULKAN"],
        "//meta_package:rdmn_gl_rasterizer_backend": [
            "RDMN_OPENGL",
        ],
    }),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    linkopts = select({
        "//meta_package:rdmn_vulkan_linux": ["-lvulkan"],
        "//meta_package:rdmn_gl_linux": [
            "-lGL",
            "-lGLX",
        ],
        "//conditions:default": [],
    }) + select({
        "@bazel_tools//src/conditions:windows": [],
        "//conditions:default": [
            "-lpthread",
            "-lX11",
        ],
    }),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = select({
        "//meta_package:rdmn_vulkan_windows": [
            "@vulkan_sdk//:vulkan"
        ],
        "//conditions:default": [],
    }) + select({
        "//meta_package:rdmn_vk_rasterizer_backend": [],
        "//meta_package:rdmn_gl_rasterizer_backend": [
            "//gl_extensions:rdmn_engine_gl_extensions",
        ],
    }) + [
        "//camera:rdmn_engine_camera",
        "//datamodel:rdmn_engine_datamodel_core",
        "//datamodel:rdmn_engine_datamodel_math",
        "//datamodel:rdmn_engine_datamodel_rendering",
        "//datamodel:rdmn_engine_datamodel_visual",
        "//parsers:rdmn_engine_parser_tga",  # will be used for skybox
        "//ticker:rdmn_engine_ticker",
        "//meta_package:rdmn_engine_config",
        "//common:rdmn_engine_common_trivial_logger",
        "//parsers:rdmn_engine_parser_x3d",  # will be used for skybox
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_RENDERER_TARGET,
    deps = [
        _RENDERER_TARGET,
    ],
    shared_lib_name = _RENDERER_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = select({
        "//meta_package:rdmn_vk_rasterizer_backend": [],
        "//meta_package:rdmn_gl_rasterizer_backend": [
            "//gl_extensions:rdmn_engine_gl_extensions_shared",
        ],
    }) + [
        "//camera:rdmn_engine_camera_shared",
        "//datamodel:rdmn_engine_datamodel_core_shared",
        "//datamodel:rdmn_engine_datamodel_math_shared",
        "//datamodel:rdmn_engine_datamodel_rendering_shared",
        "//datamodel:rdmn_engine_datamodel_visual_shared",
        "//datamodel:rdmn_engine_datamodel_geometry_shared",
        "//parsers:rdmn_engine_parser_tga_shared",
        "//ticker:rdmn_engine_ticker_shared",
        "//common:rdmn_engine_common_trivial_logger_shared",
        "//parsers:rdmn_engine_parser_x3d_shared", 
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## SO

pkg_tar(
    name = "renderer-data",
    srcs = [
        _SHARED_RENDERER_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "renderer-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":renderer-data",
    ],
)

pkg_deb(
    name = "renderer-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":renderer-package-data",
    depends = [
        "rdmn_engine_datamodel_core",
        "rdmn_engine_datamodel_math",
        "rdmn_engine_datamodel_rendering",
        "rdmn_engine_datamodel_visual",
        "rdmn_engine_ticker",
        "rdmn_engine_common_trivial_logger",
    ] + select({
        "//meta_package:rdmn_gl_linux": ["rdmn_engine_gl_extensions"],
        "//meta_package:rdmn_vulkan_linux": ["vulkan-sdk"],
        "//conditions:default": [],
    }),
    description = "RDMN game engine renderer libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _RENDERER_TARGET,
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "renderer-headers-data",
    srcs = [
        "include/renderer/Renderer.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/renderer",
)

pkg_tar(
    name = "renderer-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":renderer-headers-data",
    ],
)

pkg_deb(
    name = "renderer-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":renderer-headers-package-data",
    depends = [
        "rdmn_engine_datamodel_core_dev",
        "rdmn_engine_datamodel_math_dev",
        "rdmn_engine_datamodel_rendering_dev",
        "rdmn_engine_datamodel_visual_dev",
        "rdmn_engine_ticker_dev",
        _RENDERER_TARGET,
        "rdmn_engine_common_trivial_logger_dev",
        "nlohmann-json3-dev",
    ] + select({
        "//meta_package:rdmn_gl_linux": ["rdmn_engine_gl_extensions"],
        "//meta_package:rdmn_vulkan_linux": ["vulkan-sdk"],
        "//conditions:default": [],
    }),
    description = "RDMN game engine renderer headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_RENDERER_TARGET),
    version = ENGINE_VERSION_FULL,
)
