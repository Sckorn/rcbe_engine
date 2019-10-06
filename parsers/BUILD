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

cc_binary(
    name = "test_x3d_parser",
    srcs = ["src/test_x3d_parser.cpp"],
    deps = [":x3d_parser"],
    visibility = ["//visibility:public"],
)