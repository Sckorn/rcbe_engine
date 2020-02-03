cc_library(
    name = "engine_filesystem_utils",
    hdrs = glob([
        "include/common/utils/file_utils.h",
    ]),
    srcs = glob([
        "src/utils/file_utils.cpp",
    ]),
    includes = ["include"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "engine_fundamentals",
    hdrs = glob([
        "include/common/fundamentals/type.hpp"
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "engine_fuzzy_logic",
    hdrs = glob([
        "include/common/fuzzy_logic/*.hpp",
    ]),
    srcs = glob([
        "srcs/fuzzy_logic/**/*.cpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":engine_fundamentals"],
)