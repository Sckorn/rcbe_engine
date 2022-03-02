workspace(name = "rcbe_engine")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

skylib_version = "1.4.2"

http_archive(
    name = "bazel_skylib",
    sha256 = "66ffd9315665bfaafc96b52278f57c7e2dd09f5ede279ea6d39b2be471e7e3aa",
    urls = [
        "https://github.com/bazelbuild/bazel-skylib/releases/download/{}/bazel-skylib-{}.tar.gz".format(skylib_version, skylib_version),
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/{}/bazel-skylib-{}.tar.gz".format(skylib_version, skylib_version),
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()
# External deps

http_archive(
    name = "com_github_nelhage_rules_boost",
    url = "https://github.com/nelhage/rules_boost/archive/5d04542e52164931841d06d5a6b3fd2f43c436d0.tar.gz",
    strip_prefix = "rules_boost-5d04542e52164931841d06d5a6b3fd2f43c436d0",
    sha256 = "6c15542d0d3b6f47cd4d2a1632a3054a0502b5102e471b4453354312cb474407",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
  strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
  sha256 = "755f9a39bc7205f5a0c428e920ddad092c33c8a1b46997def3f1d4a82aded6e1",
)

http_archive(
    name = "nlohmann",
    build_file = "@//thirdparty:nlohmann.BUILD",
    sha256 = "87b5884741427220d3a33df1363ae0e8b898099fbc59f1c451113f6732891014",
    url = "https://github.com/nlohmann/json/releases/download/v3.7.3/include.zip",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

http_file(
    name = "low_poly_wolf_stl",
    downloaded_file_path = "LowPolyWolf.stl",
    sha256 = "3da76bfc52b98e7e7cc8e805345b7c54fc7d53e24cb501c1e4ae542760947aa1",
    tags = ["local"],
    urls = ["http://localhost:8080/static/LowPolyWolf.stl"],
)

http_file(
    name = "brick_wall_texture",
    downloaded_file_path = "brick_wall_texture.tga",
    sha256 = "bc1f3b97a10dbe671abd2cce7393e67094273c6fcf1c136c872380dd91ec8646",
    tags = ["local"],
    urls = [
        "http://localhost:8080/static/tex.tga",
    ],
)

http_file(
    name = "awesomeface_texture",
    downloaded_file_path = "awesomeface_texture.tga",
    sha256 = "66510d10328a164669a8d3690f98c669a72e7cfd72bc8c2cebfbcbcc5f450cd6",
    tags = ["local"],
    urls = [
        "http://localhost:8080/static/tex2.tga",
    ],
)

http_archive(
    name = "stl_test_data_archive",
    build_file = "@//thirdparty:stl_tests_archive.BUILD",
    sha256 = "8e045f19f73320715bea8162161a3107b1c3a26d6c822a252bd201836897c7de",
    tags = ["local"],
    urls = ["http://localhost:8080/static/test_stls.tar.gz"],
)

http_archive(
    name = "gltf_test_data_archive",
    build_file = "@//thirdparty:gltf_tests_archive.BUILD",
    sha256 = "8332c78756816faa99a15d85bb44db87937299b782da4a42a9c1f2f7e04e89cc",
    tags = ["local"],
    urls = ["http://localhost:8080/static/viking_room.tar.xz"],
)

RULES_PKG_VERSION = "0.9.1"

http_archive(
    name = "rules_pkg",
    sha256 = "8f9ee2dc10c1ae514ee599a8b42ed99fa262b757058f65ad3c384289ff70c4b8",
    urls = [
        "https://github.com/bazelbuild/rules_pkg/releases/download/{}/rules_pkg-{}.tar.gz".format(RULES_PKG_VERSION, RULES_PKG_VERSION),
        "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/{}/rules_pkg-{}.tar.gz".format(RULES_PKG_VERSION, RULES_PKG_VERSION),
    ],
)

http_archive(
    name = "stbi",
    build_file = "@//thirdparty:stbi.BUILD",
    sha256 = "0675244421bf802e5ffa90da42ab10280ec7c77ef2f2e999b70c9a6255d1a522",
    urls = [
        "https://github.com/nothings/stb/archive/refs/heads/master.zip",
    ],
)

load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("@rules_pkg//:rpm.bzl", "pkg_rpm")

rules_pkg_dependencies()
