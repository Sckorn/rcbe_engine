load("//meta_package:global_variables.bzl", "ENGINE_BUILD_TARGET_PREFIX")
load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")
load("//meta_package:rules.bzl", "r_cc_library")
load("//meta_package:rules.bzl", "r_cc_shared_library")


_COMMON_LINKOPTS = select({
    "@bazel_tools//src/conditions:windows": [""],
    "//conditions:default": ["-lpthread"],
})

_COMMON_INCLUDE_PATH = "include/common"

_PACKAGE_NAME = "common"

_FILESYSTEM_UTILS_TARGET = "{}_{}_filesystem_utils".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_FILESYSTEM_UTILS_TARGET = "{}_{}_filesystem_utils_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _FILESYSTEM_UTILS_TARGET,
    srcs = [
        "src/utils/file_utils.cpp",
    ],
    hdrs = [
        "include/common/utils/file_utils.hpp",
    ],
    include_prefix = "rcbe-engine",
    includes = [_COMMON_INCLUDE_PATH],
    linkopts = _COMMON_LINKOPTS,
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
)

r_cc_shared_library(
    name = _SHARED_FILESYSTEM_UTILS_TARGET,
    deps = [_FILESYSTEM_UTILS_TARGET],
    shared_lib_name = _FILESYSTEM_UTILS_TARGET,
    visibility = ["//visibility:public"],
)

_JSON_UTILS_TARGET = "{}_{}_json_utils".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_JSON_UTILS_TARGET = "{}_{}_json_utils_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _JSON_UTILS_TARGET,
    srcs = ["src/utils/json_utils.cpp"],
    hdrs = [
        "include/common/utils/json_utils.hpp",
    ],
    include_prefix = "rcbe-engine",
    includes = [_COMMON_INCLUDE_PATH],
    linkopts = _COMMON_LINKOPTS,
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = ["@nlohmann//:nlohmann_json"],
)

r_cc_shared_library(
    name = _SHARED_JSON_UTILS_TARGET,
    deps = [_JSON_UTILS_TARGET],
    shared_lib_name = _JSON_UTILS_TARGET,
    visibility = ["//visibility:public"],
)

_FUNDAMENTALS_TARGET = "{}_{}_fundamentals".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)

r_cc_library(
    name = _FUNDAMENTALS_TARGET,
    hdrs = glob([
        "include/common/fundamentals/**/*.hpp",
    ]),
    include_prefix = "rcbe-engine",
    includes = [_COMMON_INCLUDE_PATH],
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = ["//meta_package:rdmn_engine_config"],
)

_MATH_UTILS_TARGET = "{}_{}_math_utils".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)

r_cc_library(
    name = _MATH_UTILS_TARGET,
    hdrs = [
        "include/common/utils/math_utils.hpp",
    ],
    include_prefix = "rcbe-engine",
    includes = [_COMMON_INCLUDE_PATH],
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [_FUNDAMENTALS_TARGET],
)

_FUZZY_LOGIC_TARGET = "{}_{}_fuzzy_logic".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)

r_cc_library(
    name = _FUZZY_LOGIC_TARGET,
    hdrs = glob([
        "include/common/fuzzy_logic/*.hpp",
    ]),
    include_prefix = "rcbe-engine",
    includes = [_COMMON_INCLUDE_PATH],
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [_FUNDAMENTALS_TARGET],
)

_DELEGATE_TARGET = "{}_{}_delegate".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME);

r_cc_library(
    name = _DELEGATE_TARGET,
    hdrs = glob(["include/common/delegate/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = [_COMMON_INCLUDE_PATH],
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        _FUNDAMENTALS_TARGET,
    ],
)

_PROFILING_TARGET = "{}_{}_profiling".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)

# add packaging for DEV version
r_cc_library(
    name = _PROFILING_TARGET,
    hdrs = ["include/common/utils/profiling_utils.hpp"],
    include_prefix = "rcbe-engine",
    includes = [_COMMON_INCLUDE_PATH],
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
    deps = [
        _FUNDAMENTALS_TARGET,
    ],
)

_BINARY_TOOLS_TARGET = "{}_{}_binary_tools".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_BINARY_TOOLS_TARGET = "{}_{}_binary_tools_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _BINARY_TOOLS_TARGET,
    srcs = glob(["src/binary/**/*.cpp"]),
    hdrs = glob(["include/common/binary/**/*.hpp"]),
    include_prefix = "rcbe-engine",
    includes = [_COMMON_INCLUDE_PATH],
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
)

r_cc_shared_library(
    name = _SHARED_BINARY_TOOLS_TARGET,
    deps = [_BINARY_TOOLS_TARGET],
    shared_lib_name = _BINARY_TOOLS_TARGET,
    visibility = ["//visibility:public"],
)

_LOGGER_TARGET = "{}_{}_trivial_logger".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME)
_SHARED_LOGGER_TARGET = "{}_{}_trivial_logger_{}".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME, SHARED_LIB_SUFFIX)

r_cc_library(
    name = _LOGGER_TARGET,
    srcs = glob(["src/logger/**/*.cpp"]),
    hdrs = glob(["include/common/logger/**/*.hpp"]),
    include_prefix = "rdmn-engine",
    includes = [_COMMON_INCLUDE_PATH],
    strip_include_prefix = _COMMON_INCLUDE_PATH,
    visibility = ["//visibility:public"],
)

r_cc_shared_library(
    name = _SHARED_LOGGER_TARGET,
    deps = [_LOGGER_TARGET],
    shared_lib_name = _LOGGER_TARGET,
    visibility = ["//visibility:public"],
)

### tests

cc_test(
    name = "{}_{}_tests".format(ENGINE_BUILD_TARGET_PREFIX, _PACKAGE_NAME),
    srcs = glob(["test/*.cpp"]),
    visibility = ["//visibility:public"],
    deps = [
        _BINARY_TOOLS_TARGET,
        _DELEGATE_TARGET,
        _FUNDAMENTALS_TARGET,
        _LOGGER_TARGET,
        "@com_google_googletest//:gtest_main",
    ],
    dynamic_deps = [
        _SHARED_LOGGER_TARGET,
        _SHARED_BINARY_TOOLS_TARGET,
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
    name = "filesystem-data",
    srcs = [
        _SHARED_FILESYSTEM_UTILS_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
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
    description = "RDMN game engine common filesystem functionality package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _FILESYSTEM_UTILS_TARGET,
    version = ENGINE_VERSION_FULL,
)

pkg_tar(
    name = "json-data",
    srcs = [
        _SHARED_JSON_UTILS_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
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
    description = "RDMN game engine json related functionality package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _JSON_UTILS_TARGET,
    version = ENGINE_VERSION_FULL,
)

# binary-tools

pkg_tar(
    name = "common-binary-tools-data",
    srcs = [
        _SHARED_BINARY_TOOLS_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
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
    description = "RDMN game engine binary data manipulation utilities package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _BINARY_TOOLS_TARGET,
    version = ENGINE_VERSION_FULL,
)

# trivial-logger

pkg_tar(
    name = "rdmn-trivial-logger-data",
    srcs = [
        _SHARED_LOGGER_TARGET,
    ],
    mode = "0644",
    package_dir = "/usr/lib",
)

pkg_tar(
    name = "rdmn-trivial-logger-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":rdmn-trivial-logger-data",
    ],
)

pkg_deb(
    name = "rdmn-trivial-logger-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":rdmn-trivial-logger-package-data",
    description = "RDMN trivial logger lib package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = _LOGGER_TARGET,
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
    package_dir = "/usr/include/rcbe/rcbe-engine/utils",
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
    depends = [_FILESYSTEM_UTILS_TARGET],
    description = "RDMN game engine common filesystem functionality headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_FILESYSTEM_UTILS_TARGET),
    version = ENGINE_VERSION_FULL,
)

# json-dev

pkg_tar(
    name = "json-data-headers",
    srcs = [
        "include/common/utils/json_utils.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/utils",
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
    description = "RDMN game engine common json functionality headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_JSON_UTILS_TARGET),
    version = ENGINE_VERSION_FULL,
)

# fundamentals-dev

pkg_tar(
    name = "fundamentals-data-headers",
    srcs = glob([
        "include/common/fundamentals/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/fundamentals",
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
    depends = ["rdmn_engine_config"],
    description = "RDMN game engine common fundamental types and functionality headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_FUNDAMENTALS_TARGET),
    version = ENGINE_VERSION_FULL,
)

# math-utils-dev

pkg_tar(
    name = "math-utils-data-headers",
    srcs = glob([
        "include/common/utils/math_utils.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/utils",
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
    depends = ["rdmn_engine_common_fundamentals_dev"],
    description = "RDMN game engine common math functionality headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_MATH_UTILS_TARGET),
    version = ENGINE_VERSION_FULL,
)

# fuzzy-logic-dev

pkg_tar(
    name = "fuzzy-logic-data-headers",
    srcs = glob([
        "include/common/fuzzy_logic/fuzzy_logic.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/fuzzy_logic",
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
        "rcbe_engine_common_fundamentals_dev",
    ],
    description = "RDMN game engine fuzzy logics functionality headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_FUZZY_LOGIC_TARGET),
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
    package_dir = "/usr/include/rcbe/rcbe-engine/delegate",
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
        "rdmn_engine_common_fundamentals_dev",
        "rdmn_engine_common_trivial_logger_dev",
    ],
    description = "RDMN game engine abstract and concrete delegate function headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_DELEGATE_TARGET),
    version = ENGINE_VERSION_FULL,
)

# binary-tools-dev

pkg_tar(
    name = "binary-tools-data-headers",
    srcs = glob([
        "include/common/binary/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rcbe-engine/binary",
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
    description = "RDMN game engine abstract and concrete delegate function headers package.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_BINARY_TOOLS_TARGET),
    version = ENGINE_VERSION_FULL,
)

# trivial-logger-dev

pkg_tar(
    name = "trivial-logger-data-headers",
    srcs = glob(["include/common/logger/**/*.hpp"]),
    mode = "0644",
    package_dir = "/usr/include/rcbe/rdmn-engine/logger",
)

pkg_tar(
    name = "trivial-logger-dev-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":trivial-logger-data-headers",
    ],
)

pkg_deb(
    name = "trivial-logger-dev-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":trivial-logger-dev-package-data",
    description = "RDMN trivial logger lib header files.",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "{}_dev".format(_LOGGER_TARGET),
    version = ENGINE_VERSION_FULL,
)
