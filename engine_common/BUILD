cc_library(
    name = "engine_common",
    hdrs = glob([
        "include/**/*.h",
        "include/**/*.hpp"
    ]),
    srcs = glob([
        "src/**/*.cpp"
    ]),
    includes = ["include"],
    deps = ["@boost//:filesystem", "@boost//:system"],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"]
)