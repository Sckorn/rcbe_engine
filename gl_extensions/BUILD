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

### packaging

## SO

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")
load("//meta_package:global_variables.bzl", "PROJECT_URL")
load("//meta_package:global_variables.bzl", "PROJECT_MAINTAINER")

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
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":gl-extensions-package-data",
    description = "RCBE game engine gl extensions libraries package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_gl_extensions",
    version = ENGINE_VERSION,
)

## Headers

pkg_tar(
    name = "gl-extensions-headers-data",
    srcs = glob([
        "include/core/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/core/",
)

pkg_tar(
    name = "gl-extensions-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":gl-extensions-headers-data",
    ],
)

pkg_deb(
    name = "gl-extensions-headers-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":gl-extensions-headers-package-data",
    depends = ["rcbe_gl_extensions_{}_{}".format(ENGINE_VERSION, TARGET_ARCH)],
    description = "RCBE game engine gl extensions headers package",
    homepage = PROJECT_URL,
    maintainer = PROJECT_MAINTAINER,
    package = "rcbe_gl_extensions_dev",
    version = ENGINE_VERSION,
)
