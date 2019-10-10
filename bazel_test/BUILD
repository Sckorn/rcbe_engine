cc_binary(
    srcs = glob(["src/**/*.cpp"]),
    name = "bazel_test",
    deps = [
        "@boost//:program_options",
        "@boost//:filesystem",
        "@CGAL",
        "@gtest",
    ],
    visibility = ["//visibility:public"]
)

cc_test(
    name = "sample_test",
    srcs = ["test/sample_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest",
    ],
)