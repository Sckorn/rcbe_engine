
cc_library(
    name = "opengl",
    srcs = glob(["lib/x86_64-linux-gnu/libGL*.so*.*"]),
    hdrs = glob(["include/GL/**/*.h"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = ["@gcc_lib"],
)