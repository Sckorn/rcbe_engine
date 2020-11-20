cc_library(
    name = "engine_filesystem_utils",
    srcs = glob([
        "src/utils/file_utils.cpp",
    ]),
    hdrs = glob([
        "include/common/utils/file_utils.h",
    ]),
    includes = ["include"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "engine_json_utils",
    srcs = ["src/utils/json_utils.cpp"],
    hdrs = glob([
        "include/common/utils/json_utils.hpp",
    ]),
    copts = ["-Iexternal/nlohmann/include"],
    includes = ["include"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
    deps = ["@nlohmann"],
)

cc_library(
    name = "engine_fundamentals",
    hdrs = glob([
        "include/common/fundamentals/**/*.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "engine_math_utils",
    hdrs = glob([
        "include/common/utils/math_utils.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":engine_fundamentals"],
)

cc_library(
    name = "engine_stack_utils",
    hdrs = ["include/common/utils/stack_utils.hpp"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":engine_fundamentals"],
)

cc_library(
    name = "engine_output_utils",
    srcs = ["src/utils/output_utils.cpp"],
    hdrs = ["include/common/utils/output_utils.hpp"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_fundamentals",
        "@boost//:log",
    ],
)

cc_library(
    name = "engine_fuzzy_logic",
    hdrs = glob([
        "include/common/fuzzy_logic/*.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":engine_fundamentals"],
)

cc_library(
    name = "rcbe_delegate",
    hdrs = glob(["include/common/delegate/**/*.hpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_fundamentals",
        "@boost//:log",
    ],
)

cc_library(
    name = "rcbe_profiling",
    hdrs = glob(["include/common/utils/profiling_utils.hpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        ":engine_fundamentals",
    ],
)

### tests

cc_test(
    name = "common_test",
    srcs = glob(["test/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_delegate",
        "@gtest",
    ],
)

### packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")

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
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_utils_filesystem",
    version = ENGINE_VERSION,
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
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_utils_json",
    version = ENGINE_VERSION,
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
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_utils_output",
    version = ENGINE_VERSION,
)

## Headers (dev)

# filesystem-dev

pkg_tar(
    name = "filesystem-data-headers",
    srcs = [
        "include/common/utils/file_utils.h",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/common/utils",
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
    depends = ["rcbe_common_utils_filesystem_{}_{}".format(ENGINE_VERSION, TARGET_ARCH)],
    description = "RCBE game engine common filesystem functionality headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_utils_filesystem_dev",
    version = ENGINE_VERSION,
)

# json-dev

pkg_tar(
    name = "json-data-headers",
    srcs = [
        "include/common/utils/json_utils.hpp",
    ],
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/common/utils",
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
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_utils_json_dev",
    version = ENGINE_VERSION,
)

# fundamentals-dev

pkg_tar(
    name = "fundamentals-data-headers",
    srcs = glob([
        "include/common/fundamentals/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/common/fundamentals",
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
    description = "RCBE game engine common fundamental types and functionality headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_fundamentals_dev",
    version = ENGINE_VERSION,
)

# math-utils-dev

pkg_tar(
    name = "math-utils-data-headers",
    srcs = glob([
        "include/common/utils/math_utils.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/common/utils",
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
    depends = ["rcbe_common_fundamentals_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH)],
    description = "RCBE game engine common math functionality headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_math_utils_dev",
    version = ENGINE_VERSION,
)

# output-utils-dev

pkg_tar(
    name = "output-utils-data-headers",
    srcs = glob([
        "include/common/utils/output_utils.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/common/utils",
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
        "rcbe_common_fundamentals_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-log{}-dev".format(BOOST_TARGET_VERSION),
        "rcbe_common_output_utils_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine common math functionality headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_output_utils_dev",
    version = ENGINE_VERSION,
)

# fuzzy-logic-dev

pkg_tar(
    name = "fuzzy-logic-data-headers",
    srcs = glob([
        "include/common/fuzzy_logic/fuzzy_logic.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/common/fuzzy_logic",
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
        "rcbe_common_fundamentals_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
    ],
    description = "RCBE game engine fuzzy logics functionality headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_fuzzy_logic_dev",
    version = ENGINE_VERSION,
)

# delegate-dev

pkg_tar(
    name = "delegate-data-headers",
    srcs = glob([
        "include/common/delegate/AbstractDelegate.hpp",
        "include/common/delegate/Delegate.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/common/delegate",
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
        "rcbe_common_fundamentals_dev_{}_{}".format(ENGINE_VERSION, TARGET_ARCH),
        "libboost-log{}-dev".format(BOOST_TARGET_VERSION),
    ],
    description = "RCBE game engine abstract and concrete delegate function headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common_delegate_dev",
    version = ENGINE_VERSION,
)
