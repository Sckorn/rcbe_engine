load("//meta_package:global_variables.bzl", "LOGGING_LIB")
load("//meta_package:global_variables.bzl", "LOGGING_LIB_DEFINE")

_COMMON_LINKOPTS = select({
    "@bazel_tools//src/conditions:windows": [""],
    "//conditions:default": ["-lpthread"],
})

cc_library(
    name = "engine_filesystem_utils",
    srcs = [
        "src/utils/file_utils.cpp",
    ],
    hdrs = [
        "include/common/utils/file_utils.hpp",
    ],
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    linkopts = _COMMON_LINKOPTS,
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "engine_json_utils",
    srcs = ["src/utils/json_utils.cpp"],
    hdrs = [
        "include/common/utils/json_utils.hpp",
    ],
    copts = ["-Iexternal/nlohmann/include"],
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    linkopts = _COMMON_LINKOPTS,
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = ["@nlohmann//:nlohmann_json"],
)

cc_library(
    name = "engine_fundamentals",
    hdrs = glob([
        "include/common/fundamentals/**/*.hpp",
    ]),
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = ["//meta_package:rdmn_engine_config"],
)

cc_library(
    name = "engine_math_utils",
    hdrs = [
        "include/common/utils/math_utils.hpp",
    ],
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = [":engine_fundamentals"],
)

cc_library(
    name = "engine_output_utils",
    srcs = [
        "src/utils/output_utils.cpp", 
        "src/utils/boost_output_utils.hpp",
        "src/utils/glog_output_utils.hpp",
    ],
    hdrs = ["include/common/utils/output_utils.hpp"],
    defines = LOGGING_LIB_DEFINE,
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = [
        ":engine_fundamentals",
        ":rdmn_trivial_logger",
        "//meta_package/public_api:rdmn_public_api"
    ],
)

cc_library(
    name = "engine_fuzzy_logic",
    hdrs = glob([
        "include/common/fuzzy_logic/*.hpp",
    ]),
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = [":engine_fundamentals"],
)

cc_library(
    name = "rcbe_delegate",
    hdrs = glob(["include/common/delegate/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = [
        ":engine_fundamentals",
    ],
)

cc_library(
    name = "rcbe_profiling",
    hdrs = ["include/common/utils/profiling_utils.hpp"],
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = [
        ":engine_fundamentals",
    ],
)

cc_library(
    name = "rcbe_binary_tools",
    srcs = glob(["src/binary/**/*.cpp"]),
    hdrs = glob(["include/common/binary/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = ["include/common"],
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = ["//meta_package/public_api:rdmn_public_api"],
)

cc_library(
    name = "rdmn_trivial_logger",
    srcs = glob(["src/logger/**/*.cpp"]),
    hdrs = glob(["include/common/logger/**/*.hpp"]),
    defines = LOGGING_LIB_DEFINE,
    include_prefix = "rdmn-engine",
    includes = ["include/common"],
    strip_include_prefix = "include/common",
    visibility = ["//visibility:public"],
    deps = [
        "//meta_package/public_api:rdmn_public_api"
    ] + LOGGING_LIB,
)

### tests

cc_test(
    name = "rcbe_common_tests",
    srcs = glob(["test/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_binary_tools",
        ":rcbe_delegate",
        ":engine_fundamentals",
        ":rdmn_trivial_logger",
        ":engine_output_utils",
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

pkg_tar(
    name = "filesystem-data",
    srcs = [
        "//common:engine_filesystem_utils",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "common-filesystem-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":filesystem-data",
    ],
)

pkg_deb(
    name = "common-filesystem-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-filesystem-package-data",
    description = "RCBE game engine common filesystem functionality package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_utils_filesystem",
    version = ENGINE_VERSION_FULL,
)

pkg_tar(
    name = "json-data",
    srcs = [
        "//common:engine_json_utils",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "json-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":json-data",
    ],
)

pkg_deb(
    name = "json-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":json-package-data",
    description = "RCBE game engine json related functionality package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_utils_json",
    version = ENGINE_VERSION_FULL,
)

# output-utils

pkg_tar(
    name = "common-utils-output-data",
    srcs = [
        "//common:engine_output_utils",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "common-utils-output-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":common-utils-output-data",
    ],
)

pkg_deb(
    name = "common-utils-output-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-utils-output-package-data",
    depends = ["libboost-log{}.0".format(BOOST_TARGET_VERSION)],
    description = "RCBE game engine console output related utilities package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_utils_output",
    version = ENGINE_VERSION_FULL,
)

# binary-tools

pkg_tar(
    name = "common-binary-tools-data",
    srcs = [
        "//common:rcbe_binary_tools",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "common-binary-tools-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":common-binary-tools-data",
    ],
)

pkg_deb(
    name = "common-binary-tools-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-binary-tools-package-data",
    description = "RCBE game engine binary data manipulation utilities package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_binary_tools",
    version = ENGINE_VERSION_FULL,
)

## Headers (dev)

# filesystem-dev

pkg_tar(
    name = "filesystem-data-headers",
    srcs = [
        "include/common/utils/file_utils.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/utils",
)

pkg_tar(
    name = "common-filesystem-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":filesystem-data-headers",
    ],
)

pkg_deb(
    name = "common-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-filesystem-dev-package-data",
    depends = ["rcbe_common_utils_filesystem_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH)],
    description = "RCBE game engine common filesystem functionality headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_utils_filesystem_dev",
    version = ENGINE_VERSION_FULL,
)

# json-dev

pkg_tar(
    name = "json-data-headers",
    srcs = [
        "include/common/utils/json_utils.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/utils",
)

pkg_tar(
    name = "common-json-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":json-data-headers",
    ],
)

pkg_deb(
    name = "common-json-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-json-dev-package-data",
    depends = [
        "nlohmann-json3-dev",
    ],
    description = "RCBE game engine common json functionality headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_utils_json_dev",
    version = ENGINE_VERSION_FULL,
)

# fundamentals-dev

pkg_tar(
    name = "fundamentals-data-headers",
    srcs = glob([
        "include/common/fundamentals/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/fundamentals",
)

pkg_tar(
    name = "common-fundamentals-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":fundamentals-data-headers",
    ],
)

pkg_deb(
    name = "common-fundamentals-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-fundamentals-dev-package-data",
    depends = ["rcbe_engine_config_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH)],
    description = "RCBE game engine common fundamental types and functionality headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_fundamentals_dev",
    version = ENGINE_VERSION_FULL,
)

# math-utils-dev

pkg_tar(
    name = "math-utils-data-headers",
    srcs = glob([
        "include/common/utils/math_utils.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/utils",
)

pkg_tar(
    name = "common-math-utils-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":math-utils-data-headers",
    ],
)

pkg_deb(
    name = "common-math-utils-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-math-utils-dev-package-data",
    depends = ["rcbe_common_fundamentals_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH)],
    description = "RCBE game engine common math functionality headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_utils_math_dev",
    version = ENGINE_VERSION_FULL,
)

# output-utils-dev

pkg_tar(
    name = "output-utils-data-headers",
    srcs = glob([
        "include/common/utils/output_utils.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/utils",
)

pkg_tar(
    name = "common-output-utils-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":output-utils-data-headers",
    ],
)

pkg_deb(
    name = "common-output-utils-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-output-utils-dev-package-data",
    depends = [
        "rcbe_common_fundamentals_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "libboost-log{}-dev".format(BOOST_TARGET_VERSION),
        "rcbe_common_output_utils_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine common math functionality headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_utils_output_dev",
    version = ENGINE_VERSION_FULL,
)

# fuzzy-logic-dev

pkg_tar(
    name = "fuzzy-logic-data-headers",
    srcs = glob([
        "include/common/fuzzy_logic/fuzzy_logic.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/fuzzy_logic",
)

pkg_tar(
    name = "common-fuzzy-logic-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":fuzzy-logic-data-headers",
    ],
)

pkg_deb(
    name = "common-fuzzy-logic-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-fuzzy-logic-dev-package-data",
    depends = [
        "rcbe_common_fundamentals_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
    ],
    description = "RCBE game engine fuzzy logics functionality headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_fuzzy_logic_dev",
    version = ENGINE_VERSION_FULL,
)

# delegate-dev

pkg_tar(
    name = "delegate-data-headers",
    srcs = glob([
        "include/common/delegate/AbstractDelegate.hpp",
        "include/common/delegate/Delegate.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/delegate",
)

pkg_tar(
    name = "common-delegate-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":delegate-data-headers",
    ],
)

pkg_deb(
    name = "common-delegate-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":common-delegate-dev-package-data",
    depends = [
        "rcbe_common_fundamentals_dev_{}_{}".format(ENGINE_VERSION_FULL, TARGET_ARCH),
        "libboost-log{}-dev".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine abstract and concrete delegate function headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_common_delegate_dev",
    version = ENGINE_VERSION_FULL,
)

# binary-tools-dev

pkg_tar(
    name = "binary-tools-data-headers",
    srcs = glob([
        "include/common/binary/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/rcbe-engine/binary",
)

pkg_tar(
    name = "binary-tools-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":binary-tools-data-headers",
    ],
)

pkg_deb(
    name = "binary-tools-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":binary-tools-dev-package-data",
    description = "RCBE game engine abstract and concrete delegate function headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_binary_tools_dev",
    version = ENGINE_VERSION_FULL,
)
