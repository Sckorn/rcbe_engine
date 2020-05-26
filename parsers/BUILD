cc_library(
    name = "x3d_parser",
    srcs = ["src/x3d/X3dParser.cpp"],
    hdrs = ["include/parsers/x3d/X3dParser.hpp"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "//common:engine_filesystem_utils",
        "//datamodel:engine_datamodel_geometry",
        "@boost//:property_tree",
    ],
)

filegroup(
    name = "x3d_test_resources",
    srcs = glob([
        "test/resources/*",
    ]),
    visibility = ["//visibility:public"],
)

cc_test(
    name = "x3d_parser_test",
    srcs = glob(["test/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    data = glob(["test/resources/*"]),
    visibility = ["//visibility:public"],
    deps = [
        ":x3d_parser",
        "//datamodel:engine_datamodel_math",
        "@gtest",
    ],
)
