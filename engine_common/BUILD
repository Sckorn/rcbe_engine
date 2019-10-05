cc_library(
    name = "engine_filesystem_utils",
    hdrs = glob([
        "include/common/utils/file_utils.h",
    ]),
    srcs = glob([
        "src/common/utils/file_utils.cpp",
    ]),
    includes = ["include"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"]
)

#cc_library(
#    name = "engine_common",
#    hdrs = glob([
#        "include/**/*.h",
#        "include/**/*.hpp"
#    ]),
#    srcs = glob([
#        "src/**/*.cpp"
#    ]),
#    includes = ["include"],
#    deps = ["@boost//:filesystem", "@boost//:system"],
#    linkopts = ["-lpthread"],
#    visibility = ["//visibility:public"]
#