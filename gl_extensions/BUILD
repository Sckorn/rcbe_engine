cc_library(
    name = "gl_extensions",
    srcs = glob(
        ["src/gl_extensions.cpp"],
        exclude = ["src/main.cpp"],
    ),
    hdrs = glob(["include/core/*.hpp"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [],
)

# packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")

pkg_tar(
    name = "gl-extensions-data",
    srcs = [
        ":gl_extensions",
    ],
    mode = "0644",
    package_dir = "/usr/local/lib",
)

pkg_tar(
    name = "gl-extensions-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":gl-extensions-data",
    ],
)

pkg_deb(
    name = "gl-extensions-deb",
    architecture = "amd64",
    built_using = "unzip (6.0.1)",
    data = ":gl-extensions-package-data",
    description = "RCBE game engine gl extensions libraries package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_gl_extensions",
    version = ENGINE_VERSION,
)
