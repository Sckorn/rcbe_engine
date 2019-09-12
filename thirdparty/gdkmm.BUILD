cc_library(
    name = "gdkmm",
    srcs = glob([
        "lib/x86_64-linux-gnu/libgdkmm*.so*",
    ]),
    hdrs = glob([
        "lib/x86_64-linux-gnu/gdkmm-3.0/include/gdkmmconfig.h",
        "include/gdkmm-3.0/**/*.hpp",
        "include/gdkmm-3.0/**/*.h",
    ]),
    includes = ["lib/x86_64-linux-gnu/gdkmm-3.0/include/", "include/gdkmm-3.0/"],
    visibility = ["//visibility:public"],
)