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

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "c3fae06e819ed8b93e31b150387dce4864758643",
    remote = "https://github.com/nelhage/rules_boost",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

http_archive(
    name = "CGAL",
    build_file = "@//thirdparty:CGAL.BUILD",
    sha256 = "112c4e1ce8969a3c29e77ee8d58c2b9e4b3cbab5234af187207ab8ca7ddd2575",
    strip_prefix = "CGAL-4.13.1",
    url = "http://localhost:8080/static/CGAL-4.13.1.tar.gz",
)

http_archive(
    name = "gtest",
    build_file = "@//thirdparty:gtest.BUILD",
    sha256 = "b58cb7547a28b2c718d1e38aee18a3659c9e3ff52440297e965f5edffe34b6d0",
    strip_prefix = "googletest-release-1.7.0",
    url = "https://github.com/google/googletest/archive/release-1.7.0.zip",
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
