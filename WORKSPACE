workspace(name = "rcbe_engine")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

skylib_version = "1.0.3"

http_archive(
    name = "bazel_skylib",
    sha256 = "1c531376ac7e5a180e0237938a2536de0c54d93f5c278634818e0efc952dd56c",
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
    commit = "fbac9be7640ecc0fab075233d394f08f1a37e449",
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

http_archive(
    name = "rules_pkg",
    sha256 = "aeca78988341a2ee1ba097641056d168320ecc51372ef7ff8e64b139516a4937",
    urls = [
        "https://github.com/bazelbuild/rules_pkg/releases/download/0.2.6/rules_pkg-0.2.6.tar.gz",
        "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/0.2.6/rules_pkg-0.2.6.tar.gz",
    ],
)

load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("@rules_pkg//:rpm.bzl", "pkg_rpm")

rules_pkg_dependencies()
