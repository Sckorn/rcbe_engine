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
    name = "engine_fuzzy_logic",
    hdrs = glob([
        "include/common/fuzzy_logic/*.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":engine_fundamentals"],
)

# packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta-package:global_variables.bzl", "ENGINE_VERSION")

pkg_tar(
    name = "filesystem-data",
    srcs = [
        "//common:engine_filesystem_utils",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "common-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":filesystem-data",
    ],
)

pkg_deb(
    name = "common-deb",
    architecture = "amd64",
    built_using = "unzip (6.0.1)",
    data = ":common-package-data",
    depends = ["libboost-all-dev"],
    description = "RCBE game engine common functionality package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_common",
    version = ENGINE_VERSION,
)
