cc_library(
    name = "minizip",
    srcs = ["lib/x86_64-linux-gnu/libminizip.so.1"],
)

cc_library(
    name = "curl",
    srcs = glob(["lib/x86_64-linux-gnu/libcurl.so", "lib/x86_64-linux-gnu/libcurl.so*"]),
    includes = ["include/curl"],
    visibility = ["//visibility:public"],
    deps = [":minizip"],
)

cc_library(
    name = "common_deps",
    includes = ["include/x86_64-linux-gnu", "include"],
    srcs = glob(["lib/x86_64-linux-gnu/libtinyxml.so*"]),
    visibility = ["//visibility:public"],
)

#cc_library(
#    name = "ccd",
#    srcs = ["lib/x86_64-linux-gnu/libccd.so.2"],
#    visibility = ["//visibility:public"],
#)

cc_library(
    name = "x11_deps",
    srcs = glob(
        [
            "lib/x86_64-linux-gnu/libX*.so.6",
        ],
    ),
    visibility = ["//visibility:public"],
)

#cc_library(
#    name = "pcl_deps",
#    srcs = [
#        "lib/x86_64-linux-gnu/libqhull.so.7",
#    ],
#    visibility = ["//visibility:public"],
#)

cc_library(
    name = "opengl",
    srcs = glob(
        [
            "lib/x86_64-linux-gnu/mesa/libGL.so*",
        ],
    ),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "vtk_deps",
    srcs = [
        "lib/x86_64-linux-gnu/libjsoncpp.so.1",
        "lib/x86_64-linux-gnu/libmpi.so.12",
        "lib/x86_64-linux-gnu/libmpi_cxx.so.1",
        "lib/x86_64-linux-gnu/libibverbs.so.1",
        "lib/x86_64-linux-gnu/libopen-pal.so.13",
        "lib/x86_64-linux-gnu/libopen-rte.so.12",
        "lib/x86_64-linux-gnu/libhwloc.so.5",
        "lib/x86_64-linux-gnu/libnuma.so.1",
    ],
    visibility = ["//visibility:public"],
)
