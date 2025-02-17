load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")

_PACKAGE_NAME = "datamodel"

_DATAMODEL_INCLUDE_PATH = "include"

_TARGET_PACKAGE_PREFIX = "{}_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)

_CORE_TARGET = "{}_core".format(_TARGET_PACKAGE_PREFIX)
_SHARED_CORE_TARGET = "{}_core_{}".format(_TARGET_PACKAGE_PREFIX, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _CORE_TARGET,
    srcs = glob([
        "src/core/**/*.cpp",
    ]),
    hdrs = glob([
        "include/datamodel/core/**/*.hpp",
    ]),
    include_prefix = "rcbe-engine",
    includes = [_DATAMODEL_INCLUDE_PATH],
    strip_include_prefix = _DATAMODEL_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        "//common:rdmn_engine_common_fundamentals",
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_CORE_TARGET,
    deps = [_CORE_TARGET],
    shared_lib_name = _CORE_TARGET,
    visibility = ["//visibility:public"],
)

_MATH_TARGET = "{}_math".format(_TARGET_PACKAGE_PREFIX)
_SHARED_MATH_TARGET = "{}_math_{}".format(_TARGET_PACKAGE_PREFIX, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _MATH_TARGET,
    srcs = glob([
        "src/math/**/*.cpp",
    ]),
    hdrs = glob([
        "include/datamodel/math/**/*.hpp",
    ]),
    defines = [
        "RCBE_SCALAR_LINALG",
    ],
    include_prefix = "rcbe-engine",
    includes = [_DATAMODEL_INCLUDE_PATH],
    strip_include_prefix = _DATAMODEL_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        _CORE_TARGET,
        "//common:rdmn_engine_common_fuzzy_logic",
        "//common:rdmn_engine_common_math_utils",
        "//common:rdmn_engine_common_binary_tools",
        "//common:rdmn_engine_common_trivial_logger",
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_MATH_TARGET,
    deps = [_MATH_TARGET],
    shared_lib_name = _MATH_TARGET,
    visibility = ["//visibility:public"],
    dynamic_deps = [
        "//common:rdmn_engine_common_binary_tools_shared",
        "//common:rdmn_engine_common_trivial_logger_shared",
        _SHARED_CORE_TARGET
    ],
)

_VISUAL_TARGET = "{}_visual".format(_TARGET_PACKAGE_PREFIX)
_SHARED_VISUAL_TARGET = "{}_visual_{}".format(_TARGET_PACKAGE_PREFIX, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _VISUAL_TARGET,
    srcs = glob([
        "src/visual/**/*.cpp",
        "src/visual/**/*.hpp",
    ]),
    hdrs = glob([
        "include/datamodel/visual/**/*.hpp",
    ]),
    include_prefix = "rcbe-engine",
    includes = [_DATAMODEL_INCLUDE_PATH],
    linkopts = select({
        "//meta_package:rdmn_gl_rasterizer_backend": ["-lGL"],
        "//meta_package:rdmn_vk_rasterizer_backend": [],
    }) + [
        "-lpthread",
    ],
    strip_include_prefix = _DATAMODEL_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = select({
        "//meta_package:rdmn_gl_rasterizer_backend": ["//gl_extensions:rdmn_engine_gl_extensions"],
        "//meta_package:rdmn_vk_rasterizer_backend": [],
    }) + [
        _MATH_TARGET,
        "//exceptions:rcbe_exception",
        "//common:rdmn_engine_common_trivial_logger",
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_VISUAL_TARGET,
    shared_lib_name = _VISUAL_TARGET,
    deps = [_VISUAL_TARGET],
    dynamic_deps = [
        "//common:rdmn_engine_common_trivial_logger_shared",
        _SHARED_MATH_TARGET,
    ] + select({
        "//meta_package:rdmn_gl_rasterizer_backend": ["//gl_extensions:rdmn_engine_gl_extensions_shared"],
        "//meta_package:rdmn_vk_rasterizer_backend": [],
    }),
    visibility = ["//visibility:public"],
)

_GEOMETRY_TARGET = "{}_geometry".format(_TARGET_PACKAGE_PREFIX)
_SHARED_GEOMETRY_TARGET = "{}_geometry_{}".format(_TARGET_PACKAGE_PREFIX, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _GEOMETRY_TARGET,
    srcs = glob(["src/geometry/**/*.cpp"]),
    hdrs = glob([
        "include/datamodel/geometry/**/*.hpp",
    ]),
    include_prefix = "rcbe-engine",
    includes = [_DATAMODEL_INCLUDE_PATH],
    strip_include_prefix = _DATAMODEL_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        _CORE_TARGET,
        _MATH_TARGET,
        _VISUAL_TARGET,
        "//common:rdmn_engine_common_binary_tools",
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_GEOMETRY_TARGET,
    shared_lib_name = _GEOMETRY_TARGET,
    deps = [_GEOMETRY_TARGET],
    dynamic_deps = [
        "//common:rdmn_engine_common_binary_tools_shared",
        _SHARED_CORE_TARGET,
        _SHARED_MATH_TARGET,
        _SHARED_VISUAL_TARGET,
    ],
    visibility = ["//visibility:public"],
)

_SYSTEM_TARGET = "{}_system".format(_TARGET_PACKAGE_PREFIX)
_SHARED_SYSTEM_TARGET = "{}_system_{}".format(_TARGET_PACKAGE_PREFIX, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _SYSTEM_TARGET,
    srcs = glob([
        "src/system/**/*.cpp",
    ]),
    hdrs = glob([
        "include/datamodel/system/**/*.hpp",
    ]),
    include_prefix = "rcbe-engine",
    includes = [_DATAMODEL_INCLUDE_PATH],
    strip_include_prefix = _DATAMODEL_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        _CORE_TARGET,
        _MATH_TARGET,
        _VISUAL_TARGET,
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_SYSTEM_TARGET,
    shared_lib_name = _SYSTEM_TARGET,
    deps = [_SYSTEM_TARGET],
    dynamic_deps = [
        _SHARED_CORE_TARGET,
        _SHARED_MATH_TARGET,
        _SHARED_VISUAL_TARGET,
    ],
    visibility = ["//visibility:public"],
)

_RENDERING_TARGET = "{}_rendering".format(_TARGET_PACKAGE_PREFIX)
_SHARED_RENDERING_TARGET = "{}_rendering_{}".format(_TARGET_PACKAGE_PREFIX, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _RENDERING_TARGET,
    srcs = select({
        "//meta_package:rdmn_vk_rasterizer_backend": glob(
            [
                "src/rendering/**/*.cpp",
                "src/rendering/**/*.hpp",
            ],
            exclude = [
                "src/rendering/GLShader.hpp",
                "src/rendering/GLShader.cpp",
                "src/rendering/GLRasterizerTexture.hpp",
                "src/rendering/GLRasterizerTexture.cpp",
                "src/rendering/BufferObject.cpp",
                "src/rendering/ShaderProgram.cpp",
            ],
        ),
        "//meta_package:rdmn_gl_rasterizer_backend": glob(
            [
                "src/rendering/**/*.cpp",
                "src/rendering/**/*.hpp",
            ],
            exclude = [
                "src/rendering/VulkanShader.hpp",
                "src/rendering/VulkanShader.cpp",
                "src/rendering/VulkanGraphicsPipeline.cpp",
                "src/rendering/VulkanBufferObject.cpp",
                "src/rendering/VulkanRasterizerTexture.hpp",
                "src/rendering/rasterizer_texture_helpers.cpp",
            ],
        ),
    }),
    hdrs = select({
        "//meta_package:rdmn_gl_rasterizer_backend": glob(
            [
                "include/datamodel/rendering/**/*.hpp",
            ],
            exclude = [
                "include/datamodel/rendering/VulkanBufferObject.hpp",
                "include/datamodel/rendering/VulkanGraphicsPipeline.hpp",
                "include/datamodel/rendering/vk_texture_types.hpp",
                "include/datamodel/rendering/rasterizer_texture_helpers.hpp",
            ],
        ),
        "//meta_package:rdmn_vk_rasterizer_backend": glob(
            [
                "include/datamodel/rendering/**/*.hpp",
            ],
            exclude = [
                "include/datamodel/rendering/gl_texture_types.hpp",
                "include/datamodel/rendering/BufferObject.hpp",
                "include/datamodel/rendering/ShaderProgram.hpp",
            ],
        ),
    }),
    defines = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["RDMN_VULKAN"],
        "//meta_package:rdmn_gl_rasterizer_backend": ["RDMN_OPENGL"],
    }),
    include_prefix = "rcbe-engine",
    includes = [_DATAMODEL_INCLUDE_PATH],
    linkopts = select({
        "//meta_package:rdmn_vulkan_linux": ["-lvulkan"],
        "//meta_package:rdmn_gl_linux": ["-lGL"],
        "//conditions:default": [],
    }) + select({
        "@bazel_tools//src/conditions:linux": ["-lpthread"],
        "//conditions:default": [],
    }),
    strip_include_prefix = _DATAMODEL_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = select({
        "//meta_package:rdmn_gl_rasterizer_backend": ["//gl_extensions:rdmn_engine_gl_extensions"],
        "//meta_package:rdmn_vk_rasterizer_backend": [],
    }) + select({
        "//meta_package:rdmn_vulkan_windows": ["@vulkan_sdk//:vulkan"],
        "//conditions:default": [],
    }) + [
        _CORE_TARGET,
        _GEOMETRY_TARGET,
        _MATH_TARGET,
        _VISUAL_TARGET,
        "//common:rdmn_engine_common_fundamentals",
        "//common:rdmn_engine_common_trivial_logger",
        "@nlohmann//:nlohmann_json",
    ],
)

r_cc_shared_library(
    name = _SHARED_RENDERING_TARGET,
    shared_lib_name = _RENDERING_TARGET,
    deps = [_RENDERING_TARGET,],
    dynamic_deps = [
        _SHARED_CORE_TARGET,
        _SHARED_MATH_TARGET,
        _SHARED_VISUAL_TARGET,
        _SHARED_GEOMETRY_TARGET,
        "//common:rdmn_engine_common_trivial_logger_shared",
    ],
    visibility = ["//visibility:public"],
)

### File groups

filegroup(
    name = "datamodel_rendering_resources",
    srcs = glob(["data/rendering/**/*"]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "datamodel_system_resources",
    srcs = glob(["data/system/*"]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "datamodel_spv_shaders",
    srcs = glob(["data/rendering/shaders/*.spv"]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "datamodel_renderer_config",
    srcs = select({
        "//meta_package:rdmn_vulkan_linux": ["data/rendering/default_renderer_config.json"],
        "//meta_package:rdmn_vulkan_windows": ["data/rendering/windows_vulkan_renderer_config.json"],
        "//meta_package:rdmn_gl_linux": ["data/rendering/default_renderer_config.json"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "datamodel_camera_config",
    srcs = select({
        "//meta_package:rdmn_vk_rasterizer_backend": ["data/rendering/vulkan_camera_config.json"],
        "//meta_package:rdmn_gl_rasterizer_backend": ["data/rendering/default_camera_config.json"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "datamodel_window_config",
    srcs = select({
        "@bazel_tools//src/conditions:windows": ["data/system/windows_window_config.json"],
        "//conditions:default": ["data/system/default_window_config.json"],
    }),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "datamodel_input_config",
    srcs = ["data/system/default_game_input.json"],
    visibility = ["//visibility:public"],
)

### tests

cc_test(
    name = "{}_all_tests".format(_TARGET_PACKAGE_PREFIX),
    srcs = glob(
        ["test/*.cpp"],
        exclude = [
            "test/geometry_tests.cpp",
            "test/texture_tests.cpp",
        ],
    ),
    data = [
        ":datamodel_rendering_resources",
    ],
    visibility = ["//visibility:public"],
    deps = [
        _MATH_TARGET,
        _RENDERING_TARGET,
        _VISUAL_TARGET,
        "//common:rdmn_engine_common_fuzzy_logic",
        "//common:rdmn_engine_common_json_utils",
        "@com_google_googletest//:gtest_main",
        "@nlohmann//:nlohmann_json",
    ],
    dynamic_deps = [
        _SHARED_MATH_TARGET,
        _SHARED_RENDERING_TARGET,
        _SHARED_VISUAL_TARGET,
        "//common:rdmn_engine_common_json_utils_shared",
    ],
)

cc_test(
    name = "{}_geometry_tests".format(_TARGET_PACKAGE_PREFIX),
    srcs = [
        "test/datamodel_tests.cpp",
        "test/geometry_tests.cpp",
    ],
    data = [
        ":datamodel_rendering_resources",
        "@low_poly_wolf_stl//file",
    ],
    visibility = ["//visibility:public"],
    deps = [
        _MATH_TARGET,
        _RENDERING_TARGET,
        _VISUAL_TARGET,
        _GEOMETRY_TARGET,
        "//common:rdmn_engine_common_fuzzy_logic",
        "//common:rdmn_engine_common_json_utils",
        "//parsers:rdmn_engine_parser_stl",
        "@com_google_googletest//:gtest_main",
        "@nlohmann//:nlohmann_json",
    ],
    dynamic_deps = [
        _SHARED_MATH_TARGET,
        _SHARED_RENDERING_TARGET,
        _SHARED_VISUAL_TARGET,
        _SHARED_GEOMETRY_TARGET,
        "//common:rdmn_engine_common_json_utils_shared",
        "//common:rdmn_engine_common_binary_tools_shared",
        "//parsers:rdmn_engine_parser_stl_shared",
    ],
)

cc_test(
    name = "{}_textures_tests".format(_TARGET_PACKAGE_PREFIX),
    srcs = [
        "test/datamodel_tests.cpp",
        "test/texture_tests.cpp",
    ],
    data = [
        ":datamodel_rendering_resources",
        "@brick_wall_texture//file",
    ],
    visibility = ["//visibility:public"],
    deps = [
        _MATH_TARGET,
        _RENDERING_TARGET,
        _VISUAL_TARGET,
        "//parsers:rdmn_engine_parser_tga",
        "@com_google_googletest//:gtest_main",
        "@nlohmann//:nlohmann_json",
    ],
    dynamic_deps = [
        _SHARED_MATH_TARGET,
        _SHARED_RENDERING_TARGET,
        _SHARED_VISUAL_TARGET,
        "//parsers:rdmn_engine_parser_tga_shared",
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

## SO

# datamodel-core

pkg_tar(
    name = "datamodel-core-data",
    srcs = [
        _SHARED_CORE_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "datamodel-core-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-core-data",
    ],
)

pkg_deb(
    name = "datamodel-core-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-core-package-data",
    description = "RDMN game engine datamodel core libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _CORE_TARGET,
    version = ENGINE_VERSION_FULL,
)

# datamodel-geometry

pkg_tar(
    name = "datamodel-geometry-data",
    srcs = [
        _SHARED_GEOMETRY_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "datamodel-geometry-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-geometry-data",
    ],
)

pkg_deb(
    name = "datamodel-geometry-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-geometry-package-data",
    depends = [
        _CORE_TARGET,
        _VISUAL_TARGET,
    ],
    description = "RDMN game engine datamodel geometry libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _GEOMETRY_TARGET,
    version = ENGINE_VERSION_FULL,
)

# datamodel-math

pkg_tar(
    name = "datamodel-math-data",
    srcs = [
        _SHARED_MATH_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "datamodel-math-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-math-data",
    ],
)

pkg_deb(
    name = "datamodel-math-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-math-package-data",
    depends = [
        _CORE_TARGET,
    ],
    description = "RDMN game engine datamodel math libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _MATH_TARGET,
    version = ENGINE_VERSION_FULL,
)

# datamodel-rendering

pkg_tar(
    name = "datamodel-rendering-data",
    srcs = [
        _SHARED_RENDERING_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "datamodel-rendering-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-rendering-data",
    ],
)

pkg_deb(
    name = "datamodel-rendering-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-rendering-package-data",
    depends = [
        _CORE_TARGET,
        _GEOMETRY_TARGET,
        _MATH_TARGET,
        _VISUAL_TARGET,
        "rdmn_engine_common_trivial_logger",
    ] + select({
        "//meta_package:rdmn_gl_linux": ["rdmn_engine_gl_extensions"],
        "//meta_package:rdmn_vulkan_linux": ["vulkan-sdk"],
        "//conditions:default": [],
    }),
    description = "RDMN game engine datamodel rendering libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _RENDERING_TARGET,
    version = ENGINE_VERSION_FULL,
)

# datamodel-system

pkg_tar(
    name = "datamodel-system-data",
    srcs = [
        _SHARED_SYSTEM_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "datamodel-system-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-system-data",
    ],
)

pkg_deb(
    name = "datamodel-system-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-system-package-data",
    depends = [
        _CORE_TARGET,
        _MATH_TARGET,
        _VISUAL_TARGET
    ],
    description = "RDMN game engine datamodel system libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _SYSTEM_TARGET,
    version = ENGINE_VERSION_FULL,
)

# datamodel-visual

pkg_tar(
    name = "datamodel-visual-data",
    srcs = [
        _SHARED_VISUAL_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "datamodel-visual-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-visual-data",
    ],
)

pkg_deb(
    name = "datamodel-visual-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-visual-package-data",
    depends = [_MATH_TARGET],
    description = "RDMN game engine datamodel visual libraries package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _VISUAL_TARGET,
    version = ENGINE_VERSION_FULL,
)

## Headers

# datamodel-core-headers

pkg_tar(
    name = "datamodel-core-headers-data",
    srcs = glob([
        "include/datamodel/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/datamodel/core",
)

pkg_tar(
    name = "datamodel-core-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-core-headers-data",
    ],
)

pkg_deb(
    name = "datamodel-core-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-core-headers-package-data",
    depends = [
        _CORE_TARGET,
        "rdmn_engine_config",
    ],
    description = "RDMN game engine datamodel core headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_CORE_TARGET),
    version = ENGINE_VERSION_FULL,
)

# datamodel-geometry-headers

pkg_tar(
    name = "datamodel-geometry-headers-data",
    srcs = glob([
        "include/datamodel/geometry/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/datamodel/geometry",
)

pkg_tar(
    name = "datamodel-geometry-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-geometry-headers-data",
    ],
)

pkg_deb(
    name = "datamodel-geometry-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-geometry-headers-package-data",
    depends = [
        "{}_dev".format(_CORE_TARGET),
        "{}_dev".format(_VISUAL_TARGET),
        _GEOMETRY_TARGET,
    ],
    description = "RDMN game engine datamodel geometry headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_GEOMETRY_TARGET),
    version = ENGINE_VERSION_FULL,
)

# datamodel-math-headers

pkg_tar(
    name = "datamodel-math-headers-data",
    srcs = glob([
        "include/datamodel/math/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/datamodel/math",
)

pkg_tar(
    name = "datamodel-math-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-math-headers-data",
    ],
)

pkg_deb(
    name = "datamodel-math-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-math-headers-package-data",
    depends = [
        "{}_dev".format(_CORE_TARGET),
        _MATH_TARGET,
    ],
    description = "RDMN game engine datamodel math headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_MATH_TARGET),
    version = ENGINE_VERSION_FULL,
)

# datamodel-rendering-headers

pkg_tar(
    name = "datamodel-rendering-headers-data",
    srcs = glob([
        "include/datamodel/rendering/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/datamodel/rendering",
)

pkg_tar(
    name = "datamodel-rendering-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-rendering-headers-data",
    ],
)

pkg_deb(
    name = "datamodel-rendering-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-rendering-headers-package-data",
    depends = [
        "{}_dev".format(_CORE_TARGET),
        "{}_dev".format(_GEOMETRY_TARGET),
        "{}_dev".format(_MATH_TARGET),
        "{}_dev".format(_GEOMETRY_TARGET),
        _RENDERING_TARGET,
        "rdmn_engine_gl_extensions_dev",
        "rdmn_engine_common_trivial_logger_dev",
    ],
    description = "RDMN game engine datamodel rendering headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_RENDERING_TARGET),
    version = ENGINE_VERSION_FULL,
)

# datamodel-system-headers

pkg_tar(
    name = "datamodel-system-headers-data",
    srcs = glob([
        "include/datamodel/system/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/datamodel/system",
)

pkg_tar(
    name = "datamodel-system-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-system-headers-data",
    ],
)

pkg_deb(
    name = "datamodel-system-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-system-headers-package-data",
    depends = [
        "{}_dev".format(_CORE_TARGET),
        "{}_dev".format(_MATH_TARGET),
        "{}_dev".format(_VISUAL_TARGET),
        _SYSTEM_TARGET,
    ],
    description = "RDMN game engine datamodel system headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_SYSTEM_TARGET),
    version = ENGINE_VERSION_FULL,
)

# datamodel-visual-headers

pkg_tar(
    name = "datamodel-visual-headers-data",
    srcs = [
        "include/datamodel/visual/RGBAColor.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/datamodel/visual",
)

pkg_tar(
    name = "datamodel-visual-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":datamodel-visual-headers-data",
    ],
)

pkg_deb(
    name = "datamodel-visual-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":datamodel-visual-headers-package-data",
    depends = [
        "{}_dev".format(_MATH_TARGET),
        _VISUAL_TARGET,
    ],
    description = "RDMN game engine datamodel visual headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_VISUAL_TARGET),
    version = ENGINE_VERSION_FULL,
)
