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
    srcs = glob([
        "src/fuzzy_logic/**/*.cpp",
    ]),
    hdrs = glob([
        "include/common/fuzzy_logic/*.hpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":engine_fundamentals"],
)
