cc_library(
    name = "glib",
    srcs = glob([
        "lib/x86_64-linux-gnu/libglib-2.0*.so*",
    ]),
    hdrs = glob([
        "lib/x86_64-linux-gnu/glib-2.0/include/**/*.h",
        "include/glib-2.0/**/*.h",
    ]),
    includes = ["lib/x86_64-linux-gnu/glib-2.0/include/", "include/glib-2.0/"],
    visibility = ["//visibility:public"],
)