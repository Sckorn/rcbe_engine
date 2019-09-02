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
    #deps = ["@boost//:filesystem"],
    deps = [
        "@opengl//:opengl",
        "//thirdparty/nlohmann",
        ],
    visibility = ["//visibility:public"]
)