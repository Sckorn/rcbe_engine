# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "local_boost",
    srcs = glob( [
        "lib/x86_64-linux-gnu/libboost*.so*",
        "lib/x86_64-linux-gnu/libboost*.so",
        "lib/x86_64-linux-gnu/libboost*.a*",
    ]),
    hdrs = glob([
        "include/boost/**/*.h",
        "include/boost/**/*.hpp",
        "include/boost/**/*.hh",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
)