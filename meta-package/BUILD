# packaging

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load(":global_variables.bzl", "ENGINE_VERSION")

pkg_tar(
    name = "all-package-data",
    extension = "tar.gz",
    deps = [
        "//common:common-package-data",
        "//datamodel:datamodel-package-data",
        "//gl_extensions:gl-extensions-package-data",
        "//parsers:parsers-package-data",
        "//renderer:renderer-package-data",
        "//window_manager:wm-package-data",
    ],
)

pkg_deb(
    name = "all-deb",
    architecture = "amd64",
    built_using = "unzip (6.0.1)",
    data = ":all-package-data",
    description = "RCBE game engine meta package containing all engine libraries",
    homepage = "http://rcbe.com",
    maintainer = "Daniil Kireev <sckorn@gmail.com>",
    package = "rcbe_game_engine_all",
    version = ENGINE_VERSION,
)
