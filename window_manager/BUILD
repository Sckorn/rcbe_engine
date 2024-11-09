load("//meta_package:global_variables.bzl", "PUBLIC_API_PREFIX")

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

cc_library(
    name = "rcbe_window_manager",
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
    defines = PUBLIC_API_PREFIX,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_json_utils",
        "//datamodel:engine_datamodel_core",
        "//datamodel:engine_datamodel_math",
        "//datamodel:engine_datamodel_system",
        "//datamodel:engine_datamodel_visual",
        "//datamodel:engine_datamodel_rendering",
        "//input_manager:rcbe_input_manager",
        "//renderer:rdmn_renderer",
        "//common:rdmn_trivial_logger",
        "@nlohmann//:nlohmann_json",
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

pkg_tar(
    name = "window-manager-data",
    srcs = [
        ":rcbe_window_manager",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
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
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_system_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_visual_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_input_manager_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rdmn_renderer_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "libboost-log{}.0".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine window manager libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_window_manager",
    version = ENGINE_VERSION_FULL,
)

## Headers

pkg_tar(
    name = "window-manager-headers-data",
    srcs = glob([
        "include/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/core",
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
        "rcbe_datamodel_core_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_system_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_datamodel_visual_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_input_manager_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rdmn_renderer_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "rcbe_window_manager_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "libboost-log{}-dev".format(BOOST_TARGET_VERSION),
        "nlohmann-json3-dev",
    ],
    description = "RCBE game engine window manager headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_window_manager_dev",
    version = ENGINE_VERSION_FULL,
)
