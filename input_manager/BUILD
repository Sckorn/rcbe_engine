load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")

_PACKAGE_NAME = "input_manager"

_INPUT_MNGR_TARGET = "{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_INPUT_MNGR_TARGET = "{}_{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _INPUT_MNGR_TARGET,
    srcs = glob(["src/**/*.cpp"]),
    hdrs = [
        "include/core/AbstractInputManager.hpp", 
        "include/core/EditorInputManager.hpp", 
        "include/core/GameInputManager.hpp", 
        "include/core/InputManagerImplementation.hpp", 
        "include/core/InputManagerTraits.hpp",
    ] + select({
        "@bazel_tools//src/conditions:windows": ["include/core/windows_input_event.hpp"],
        "//conditions:default": []
    }),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    linkopts = select({
       "@bazel_tools//src/conditions:windows": [],
        "//conditions:default": [
            "-lpthread",
            "-lGL",
            "-lGLX",
            "-lX11",
        ], 
    }),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = select({
        "//meta_package:rdmn_gl_rasterizer_backend": ["//gl_extensions:rdmn_engine_gl_extensions"],
        "//meta_package:rdmn_vk_rasterizer_backend": [],
    }) + [
        "//common:rdmn_engine_common_delegate",
        "//datamodel:rdmn_engine_datamodel_system",
        "//common:rdmn_engine_common_trivial_logger",
    ],
)

r_cc_shared_library(
    name = _SHARED_INPUT_MNGR_TARGET,
    deps = [_INPUT_MNGR_TARGET],
    shared_lib_name = _INPUT_MNGR_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = select({
        "//meta_package:rdmn_gl_rasterizer_backend": ["//gl_extensions:rdmn_engine_gl_extensions_shared"],
        "//meta_package:rdmn_vk_rasterizer_backend": [],
    }) + [
        "//datamodel:rdmn_engine_datamodel_system_shared",
        "//common:rdmn_engine_common_trivial_logger_shared",
    ],
)

### Packaging

## SO

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

pkg_tar(
    name = "input-manager-data",
    srcs = [
        _SHARED_INPUT_MNGR_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
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
        "rdmn_engine_datamodel_system",
        "rdmn_engine_common_trivial_logger",
    ],
    description = "RDMN game engine input manager libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _INPUT_MNGR_TARGET,
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "input-manager-headers-data",
    srcs = glob([
        "include/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/core/",
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
    depends = [_INPUT_MNGR_TARGET],
    description = "RDMN game engine input manager headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_INPUT_MNGR_TARGET),
    version = ENGINE_VERSION_FULL,
)
