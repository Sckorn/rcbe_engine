cc_library(
    name = "x3d_parser",
    hdrs = ["include/parsers/x3d/X3dParser.hpp"],
    srcs = ["src/x3d/X3dParser.cpp"],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "@boost//:property_tree",
        "//engine_common:engine_filesystem_utils",
        "//engine_datamodel:engine_datamodel_geometry",
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
    deps = [
        ":x3d_parser",
        "@gtest",
        "//engine_datamodel:engine_datamodel_math"
    ],
    copts = ["-Iexternal/gtest/include"],
    visibility = ["//visibility:public"],
    data = glob(["test/resources/*"]),
)