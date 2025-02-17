load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")

_PACKAGE_NAME = "window_manager"

_WINDOWS_COPTS = [
    "/c",
    "/Z7",
    "/nologo",
    "/WX-",
    "/diagnostics:column",
    "/sdl", 
    "/O2", 
    "/Oi", 
    "/DNDEBUG", 
    "/D_WINDOWS", 
    "/D_UNICODE", 
    "/DUNICODE", 
    "/Gm-",
    "/EHsc",
    "/MD",
    "/GS", 
    "/Gy",
    "/GL",
    "/fp:precise", 
    "/Zc:wchar_t", 
    "/Zc:forScope", 
    "/Zc:inline", 
    "/permissive-", 
    "/external:W3",
    "/Gd", 
    "/TP", 
    "/FC",
    "/FS",
    "/errorReport:prompt",
]

_WINDOWS_LINKOPTS = [
    "/ERRORREPORT:PROMPT", 
    "/INCREMENTAL:NO", 
    "/NOLOGO", 
    "kernel32.lib", 
    "user32.lib",
    "gdi32.lib",
    "winspool.lib",
    "comdlg32.lib",
    "advapi32.lib",
    "shell32.lib",
    "ole32.lib",
    "oleaut32.lib",
    "uuid.lib",
    "odbc32.lib",
    "odbccp32.lib",
    "/MANIFEST",
    "/DEBUG",
    "/SUBSYSTEM:WINDOWS",
    "/OPT:REF",
    "/OPT:ICF",
    "/TLBID:1",
    "/DYNAMICBASE",
    "/NXCOMPAT",
    "/MACHINE:X64"
]

_MANAGER_TARGET = "{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_MANAGER_TARGET = "{}_{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _MANAGER_TARGET,
    srcs = [
        "src/RWindow.cpp",
        "src/WindowManager.cpp",
    ] + select({
        "@bazel_tools//src/conditions:windows": [
            "src/MsWindow.hpp",
            "src/MsWindow.cpp",
        ],
        "//conditions:default": [
            "src/XWindow.hpp",
            "src/XWindow.cpp",
        ]

    }),
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include"],
    copts = select({
       "@bazel_tools//src/conditions:windows": _WINDOWS_COPTS,
        "//conditions:default": [],
    }),
    linkopts = select({
        "@bazel_tools//src/conditions:windows": _WINDOWS_LINKOPTS,
        "//conditions:default": [
            "-lpthread",
            "-lX11",
            "-lXext",
        ], 
    }),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:rdmn_engine_common_json_utils",
        "//common:rdmn_engine_common_trivial_logger",
        "//datamodel:rdmn_engine_datamodel_core",
        "//datamodel:rdmn_engine_datamodel_math",
        "//datamodel:rdmn_engine_datamodel_system",
        "//datamodel:rdmn_engine_datamodel_visual",
        "//datamodel:rdmn_engine_datamodel_rendering",
        "//input_manager:rdmn_engine_input_manager",
        "//renderer:rdmn_engine_renderer",
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_MANAGER_TARGET,
    deps = [_MANAGER_TARGET],
    shared_lib_name = _MANAGER_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = [
        "//common:rdmn_engine_common_json_utils_shared",
        "//common:rdmn_engine_common_trivial_logger_shared",
        "//datamodel:rdmn_engine_datamodel_core_shared",
        "//datamodel:rdmn_engine_datamodel_math_shared",
        "//datamodel:rdmn_engine_datamodel_system_shared",
        "//datamodel:rdmn_engine_datamodel_visual_shared",
        "//datamodel:rdmn_engine_datamodel_rendering_shared",
        "//input_manager:rdmn_engine_input_manager_shared",
        "//renderer:rdmn_engine_renderer_shared",
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
    name = "window-manager-data",
    srcs = [
        _SHARED_MANAGER_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "wm-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":window-manager-data",
    ],
)

pkg_deb(
    name = "window-manager-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":wm-package-data",
    depends = [
        "rdmn_engine_datamodel_core",
        "rdmn_engine_datamodel_math",
        "rdmn_engine_datamodel_system",
        "rdmn_engine_datamodel_visual",
        "rdmn_engine_input_manager",
        "rdmn_engine_renderer",
        "rdmn_engine_common_trivial_logger",
    ],
    description = "RDMN game engine window manager libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _MANAGER_TARGET,
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "window-manager-headers-data",
    srcs = glob([
        "include/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/core",
)

pkg_tar(
    name = "wm-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":window-manager-headers-data",
    ],
)

pkg_deb(
    name = "window-manager-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":wm-headers-package-data",
    depends = [
        "rdmn_engine_datamodel_core_dev",
        "rdmn_engine_datamodel_math_dev",
        "rdmn_engine_datamodel_system_dev",
        "rdmn_engine_datamodel_visual_dev",
        "rdmn_engine_input_manager_dev",
        "rdmn_engine_renderer_dev",
        _MANAGER_TARGET,
        "rdmn_engine_common_trivial_logger_dev",
        "nlohmann-json3-dev",
    ],
    description = "RDMN game engine window manager headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_MANAGER_TARGET),
    version = ENGINE_VERSION_FULL,
)
