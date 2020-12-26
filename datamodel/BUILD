cc_library(
    name = "engine_datamodel_core",
    srcs = glob([
        "src/core/**/*.cpp",
    ]),
    hdrs = glob([
        "include/datamodel/core/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_fundamentals",
        "@nlohmann",
    ],
)

cc_library(
    name = "engine_datamodel_math",
    srcs = glob([
        "src/math/**/*.cpp",
    ]),
    hdrs = glob([
        "include/datamodel/math/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    defines = ["RCBE_SCALAR_LINALG"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        "//common:engine_fuzzy_logic",
        "//common:engine_math_utils",
        "//common:rcbe_binary_tools",
        "@nlohmann",
    ],
)

cc_library(
    name = "engine_datamodel_visual",
    srcs = glob([
        "src/visual/**/*.cpp",
    ]),
    hdrs = glob([
        "include/datamodel/visual/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_math",
        "@nlohmann",
    ],
)

cc_library(
    name = "engine_datamodel_geometry",
    srcs = glob(["src/geometry/**/*.cpp"]),
    hdrs = glob([
        "include/datamodel/geometry/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        ":engine_datamodel_math",
        ":engine_datamodel_visual",
        "//common:rcbe_binary_tools",
        "@nlohmann",
    ],
)

cc_library(
    name = "engine_datamodel_system",
    srcs = glob([
        "src/system/**/*.cpp",
    ]),
    hdrs = glob([
        "include/datamodel/system/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        ":engine_datamodel_math",
        ":engine_datamodel_visual",
        "@boost//:bimap",
        "@nlohmann",
    ],
)

cc_library(
    name = "engine_datamodel_rendering",
    srcs = glob([
        "src/rendering/**/*.cpp",
    ]),
    hdrs = glob([
        "include/datamodel/rendering/**/*.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    include_prefix = "rcbe-engine",
    includes = ["include"],
    linkopts = [
        "-lpthread",
        "-lOpenGL",
        "-lGL",
    ],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_core",
        ":engine_datamodel_geometry",
        ":engine_datamodel_math",
        ":engine_datamodel_visual",
        "//gl_extensions",
        "@boost//:log",
        "@nlohmann",
    ],
)

filegroup(
    name = "datamodel_rendering_resources",
    srcs = glob(["data/rendering/*"]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "datamodel_system_resources",
    srcs = glob(["data/system/*"]),
    visibility = ["//visibility:public"],
)

### tests

cc_test(
    name = "rcbe_all_datamodel_tests",
    srcs = glob(
        ["test/*.cpp"],
        exclude = ["test/geometry_tests.cpp"],
    ),
    copts = ["-Iexternal/gtest/include"],
    data = [
        ":datamodel_rendering_resources",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_math",
        ":engine_datamodel_rendering",
        ":engine_datamodel_visual",
        "//common:engine_fuzzy_logic",
        "//common:engine_json_utils",
        "@gtest",
        "@nlohmann",
    ],
)

cc_test(
    name = "rcbe_geometry_datamodel_tests",
    srcs = [
        "test/datamodel_tests.cpp",
        "test/geometry_tests.cpp",
    ],
    copts = ["-Iexternal/gtest/include"],
    data = [
        ":datamodel_rendering_resources",
        "@low_poly_wolf_stl//file",
    ],
    tags = ["local"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_datamodel_math",
        ":engine_datamodel_rendering",
        ":engine_datamodel_visual",
        "//common:engine_fuzzy_logic",
        "//common:engine_json_utils",
        "@gtest",
        "@nlohmann",
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

# datamodel-core

pkg_tar(
    name = "datamodel-core-data",
    srcs = [
        ":engine_datamodel_core",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
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
    description = "RCBE game engine datamodel core libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_core",
    version = ENGINE_VERSION,
)

# datamodel-geometry

pkg_tar(
    name = "datamodel-geometry-data",
    srcs = [
        ":engine_datamodel_geometry",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
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
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine datamodel geometry libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_geometry",
    version = ENGINE_VERSION,
)

# datamodel-math

pkg_tar(
    name = "datamodel-math-data",
    srcs = [
        ":engine_datamodel_math",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
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
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine datamodel math libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_math",
    version = ENGINE_VERSION,
)

# datamodel-rendering

pkg_tar(
    name = "datamodel-rendering-data",
    srcs = [
        ":engine_datamodel_rendering",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
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
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_geometry_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_gl_extensions_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-log{}.0".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine datamodel rendering libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_rendering",
    version = ENGINE_VERSION,
)

# datamodel-system

pkg_tar(
    name = "datamodel-system-data",
    srcs = [
        ":engine_datamodel_system",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
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
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-container{}.0".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine datamodel system libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_system",
    version = ENGINE_VERSION,
)

# datamodel-visual

pkg_tar(
    name = "datamodel-visual-data",
    srcs = [
        ":engine_datamodel_visual",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
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
    depends = ["rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION, TARGET_ARCH)],
    description = "RCBE game engine datamodel visual libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_visual",
    version = ENGINE_VERSION,
)

## Headers

# datamodel-core-headers

pkg_tar(
    name = "datamodel-core-headers-data",
    srcs = glob([
        "include/datamodel/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/datamodel/core",
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
        "rcbe_datamodel_core_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_config_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine datamodel core headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_core_dev",
    version = ENGINE_VERSION,
)

# datamodel-geometry-headers

pkg_tar(
    name = "datamodel-geometry-headers-data",
    srcs = glob([
        "include/datamodel/geometry/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/datamodel/geometry",
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
        "rcbe_datamodel_core_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_geometry_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine datamodel geometry headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_geometry_dev",
    version = ENGINE_VERSION,
)

# datamodel-math-headers

pkg_tar(
    name = "datamodel-math-headers-data",
    srcs = glob([
        "include/datamodel/math/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/datamodel/math",
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
        "rcbe_datamodel_core_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine datamodel math headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_math_dev",
    version = ENGINE_VERSION,
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
        "rcbe_datamodel_core_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_geometry_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_rendering_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_gl_extensions_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-log{}-dev".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine datamodel rendering headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_rendering_dev",
    version = ENGINE_VERSION,
)

# datamodel-system-headers

pkg_tar(
    name = "datamodel-system-headers-data",
    srcs = glob([
        "include/datamodel/system/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/datamodel/system",
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
        "rcbe_datamodel_core_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_system_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-container{}-dev".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine datamodel system headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_system_dev",
    version = ENGINE_VERSION,
)

# datamodel-visual-headers

pkg_tar(
    name = "datamodel-visual-headers-data",
    srcs = [
        "include/datamodel/visual/RGBAColor.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/datamodel/visual",
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
        "rcbe_datamodel_math_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "rcbe_datamodel_visual_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine datamodel visual headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_datamodel_visual_dev",
    version = ENGINE_VERSION,
)
