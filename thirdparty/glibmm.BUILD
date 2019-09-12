cc_library(
    name = "glibmm",
    srcs = glob([
        "lib/x86_64-linux-gnu/libglibmm*.so*",
    ]),
    hdrs = glob([
        "lib/x86_64-linux-gnu/glibmm-2.4/include/**/*.h",
        "include/glibmm-2.4/**/*.h",
    ]),
    includes = ["lib/x86_64-linux-gnu/glibmm-2.4/include/", "include/glibmm-2.4/"],
    visibility = ["//visibility:public"],
    deps = ["@glib"],
)