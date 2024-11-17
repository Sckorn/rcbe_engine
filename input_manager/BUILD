load("//meta_package:global_variables.bzl", "PUBLIC_API_PREFIX")

cc_library(
    name = "rcbe_input_manager",
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
       "@bazel_tools//src/conditions:windows": [""],
        "//conditions:default": [
            "-lpthread",
            "-lGL",
            "-lGLX",
            "-lX11",
        ], 
    }),
    defines = PUBLIC_API_PREFIX,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = select({
        "//meta_package:rdmn_gl_rasterizer_backend": ["//gl_extensions"],
        "//meta_package:rdmn_vk_rasterizer_backend": [],
    }) + [
        "//common:rcbe_delegate",
        "//datamodel:engine_datamodel_system",
        "//common:rdmn_trivial_logger",
    ],
)

### Packaging

## SO

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
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
        "rcbe_datamodel_system_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "libboost-log{}.0".format(BOOST_TARGET_VERSION),
        "libboost-container{}.0".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine input manager libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_input_manager",
    version = ENGINE_VERSION_FULL,
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
    depends = ["rcbe_input_manager_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH)],
    description = "RCBE game engine input manager headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_input_manager_dev",
    version = ENGINE_VERSION_FULL,
)
