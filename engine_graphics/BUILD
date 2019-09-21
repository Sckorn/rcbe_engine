cc_library(
    name = "engine_graphics",
    srcs = glob([
        "src/**/*.cpp",
    ],
    exclude = ["src/core_test_node.cpp"],
    ),
    hdrs = glob([
        "include/**/*.hpp",
        "include/**/*.h",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        #"@opengl",
        #"@gcc_lib",
        "//engine_datamodel:engine_datamodel",
    ],
)

#cc_binary(
#    name = "core_test_node",
#    srcs = ["src/core_test_node.cpp"],
#    deps = [
#        ":engine_graphics",
#        "@boost//:filesystem",
#        "@boost//:system",
#    ],
#    linkopts = ["-lpthread"],
#    visibility = ["//visibility:public"],
#)