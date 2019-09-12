cc_library(
    name = "gtkmm",
    srcs = glob([
        "lib/x86_64-linux-gnu/libgtkmm*.so*",
    ]),
    hdrs = glob([
        "lib/x86_64-linux-gnu/gtkmm-3.0/include/gtkmmconfig.h",
        "include/gtkmm-3.0/**/*.hpp",
        "include/gtkmm-3.0/**/*.h",
    ]),
    includes = ["lib/x86_64-linux-gnu/gtkmm-3.0/include/", "include/gtkmm-3.0/"],
    visibility = ["//visibility:public"],
    deps = [
        "@glibmm",
        "@sigpp",
        "@gdkmm",
    ],
)