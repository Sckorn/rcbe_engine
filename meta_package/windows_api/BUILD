load("//meta_package:rdmn_build_constants.bzl", "LIB_INCLUDE_PREFIX")

cc_library(
    name = "rdmn_public_api",
    srcs = [],
    hdrs = ["public_api.hpp"],
    include_prefix = LIB_INCLUDE_PREFIX,
    includes = ["."],
    visibility = ["//visibility:public"],
    defines = select({
        "//meta_package:rdmn_shared_libs_build": [
            "RDMN_BUILD_SHARED_LIBS",
        ],
        "//meta_package:rdmn_static_libs_build": [],
    }),
)