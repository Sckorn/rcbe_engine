cc_library(
    name = "rcbe_traits",
    hdrs = glob(["include/**/*.hpp"]),
    include_prefix = "rcbe",
    includes = ["include"],
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [],
)

### Packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//meta_package:global_variables.bzl", "ENGINE_VERSION")
load("//meta_package:global_variables.bzl", "TARGET_ARCH")
load("//meta_package:global_variables.bzl", "BOOST_TARGET_VERSION")

## Headers

pkg_tar(
    name = "type-traits-headers-data",
    srcs = glob([
        "include/traits/**/*.hpp",
    ]),
    mode = "0644",
    package_dir = "/usr/local/include/rcbe/traits",
)

pkg_tar(
    name = "type-traits-headers-package-data",
    extension = "tar.gz",
    visibility = ["//visibility:public"],
    deps = [
        ":type-traits-headers-data",
    ],
)

pkg_deb(
    name = "type-traits-header-deb",
    architecture = TARGET_ARCH,
    built_using = "unzip (6.0.1)",
    data = ":type-traits-headers-package-data",
    description = "RCBE game engine type traits headers package",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_type_traits_dev",
    version = ENGINE_VERSION,
)
