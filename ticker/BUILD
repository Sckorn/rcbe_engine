cc_library(
    name = "rcbe_ticker",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.hpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = ["//traits:rcbe_traits"],
)

# tests

cc_test(
    name = "rcbe_ticker_tests",
    srcs = glob(["test/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    visibility = ["//visibility:public"],
    deps = [
        ":rcbe_ticker",
        "@gtest",
    ],
)
