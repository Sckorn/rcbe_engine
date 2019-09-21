cc_library(
    name = "cairomm",
    srcs = glob([
        "lib/x86_64-linux-gnu/libcairo*.so*",
        "lib/x86_64-linux-gnu/libcairo*.a*",
    ]),
    hdrs = glob([
        "lib/x86_64-linux-gnu/cairomm-1.0/include/cairommconfig.h",
        "include/cairomm-1.0/**/*.hpp",
        "include/cairomm-1.0/**/*.h",
    ]),
    includes = ["lib/x86_64-linux-gnu/cairomm-1.0/include/", "include/cairomm-1.0/"],
    visibility = ["//visibility:public"],
)