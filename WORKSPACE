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
    name = "rules_7zip",
    strip_prefix = "rules_7zip-9addcf58cdc5022fa5188b5f270bc0c9bb240f46",
    urls = ["https://github.com/zaucy/rules_7zip/archive/9addcf58cdc5022fa5188b5f270bc0c9bb240f46.zip"],
    sha256 = "89db47017a95de6fc30acd0ff0b70c25e468a21b5e1286940feb21505854f28b",
)

load("@rules_7zip//:setup.bzl", "setup_7zip")

setup_7zip()

git_repository(
    name = "com_github_zaucy_rules_vulkan",
    remote = "https://github.com/Sckorn/rules_vulkan.git",
    commit = "66ca0ad5666b79feedc99a4b2a637f94c25a3a76",
)

load("@com_github_zaucy_rules_vulkan//:repo.bzl", "vulkan_repos")

vulkan_repos()

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

http_archive(
    name = "stbi",
    build_file = "@//thirdparty:stbi.BUILD",
    sha256 = "0675244421bf802e5ffa90da42ab10280ec7c77ef2f2e999b70c9a6255d1a522",
    urls = [
        "https://github.com/nothings/stb/archive/refs/heads/master.zip",
    ],
)

http_archive(
    name = "tinyxml2",
    build_file = "@//thirdparty:tinyxml2.BUILD",
    sha256 = "3bdf15128ba16686e69bce256cc468e76c7b94ff2c7f391cc5ec09e40bff3839",
    urls = [
        "https://github.com/leethomason/tinyxml2/archive/refs/tags/10.0.0.tar.gz",
    ],
    strip_prefix = "tinyxml2-10.0.0",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

http_file(
    name = "low_poly_wolf_stl",
    downloaded_file_path = "LowPolyWolf.stl",
    sha256 = "3da76bfc52b98e7e7cc8e805345b7c54fc7d53e24cb501c1e4ae542760947aa1",
    urls = [
        "https://www.dropbox.com/scl/fi/7w4ny3l2e8823gi8a901g/LowPolyWolf.stl?rlkey=uqgb5y8qf8xymbomb1ho4gn1m&st=04xsg7uo&dl=1",
        "http://localhost:8080/static/LowPolyWolf.stl",
        "file:../test_data/LowPolyWolf.stl"
    ],
)

http_file(
    name = "brick_wall_texture",
    downloaded_file_path = "brick_wall_texture.tga",
    sha256 = "bc1f3b97a10dbe671abd2cce7393e67094273c6fcf1c136c872380dd91ec8646",
    urls = [
        "https://www.dropbox.com/scl/fi/g4y8mv954l68rq8ztrz5u/tex.tga?rlkey=41vlint5apc4x594rxl8gm0ik&st=1uucv6rg&dl=1",
        "http://localhost:8080/static/tex.tga",
        "file:../test_data/tex.tga"
    ],
)

http_file(
    name = "awesomeface_texture",
    downloaded_file_path = "awesomeface_texture.tga",
    sha256 = "66510d10328a164669a8d3690f98c669a72e7cfd72bc8c2cebfbcbcc5f450cd6",
    urls = [
        "https://www.dropbox.com/scl/fi/678kxgtxq0amq42qhmwaz/tex2.tga?rlkey=3leu2lf3l2j72t3ewnqjcqoez&st=k4ksvz54&dl=1",
        "http://localhost:8080/static/tex2.tga",
        "file:../test_data/tex2.tga"
    ],
)

http_archive(
    name = "stl_test_data_archive",
    build_file = "@//thirdparty:stl_tests_archive.BUILD",
    sha256 = "8e045f19f73320715bea8162161a3107b1c3a26d6c822a252bd201836897c7de",
    urls = [
        "https://www.dropbox.com/scl/fi/yn1w69g7b7g2gze574f7f/test_stls.tar.gz?rlkey=1a97h04xjtfp7q2knaj2z06ls&st=sciag8hn&dl=1",
        "http://localhost:8080/static/test_stls.tar.gz",
        "file:../test_data/test_stls.tar.gz"
    ],
)

http_archive(
    name = "gltf_test_data_archive",
    build_file = "@//thirdparty:gltf_tests_archive.BUILD",
    sha256 = "8332c78756816faa99a15d85bb44db87937299b782da4a42a9c1f2f7e04e89cc",
    urls = [
        "https://www.dropbox.com/scl/fi/h1g0idp3dkxoip38gnijm/viking_room.tar.xz?rlkey=7bz8t4on2twirsk381o9lcjxe&st=b5ogqlti&dl=1",
        "http://localhost:8080/static/viking_room.tar.xz",
        "file:../test_data/viking_room.tar.xz"
    ],
)

RULES_WIX_COMMIT = "3c16deac685466a380d555f186de61d07c7d2cbe"

http_archive(
    name = "rules_wix",
    url = "https://github.com/mjbots/rules_wix/archive/{}.zip".format(RULES_WIX_COMMIT),

    # Replace this with the value from the bazel error message.
    sha256 = "6d6d0fd3f76496d44df7195989e9eba36bc73727e07a9a013fff2977018ff949",

    strip_prefix = "rules_wix-{}".format(RULES_WIX_COMMIT),
)

load("@rules_wix//:deps.bzl", "add_wix_deps")
add_wix_deps()

RULES_PKG_VERSION = "0.9.1"

http_archive(
    name = "rules_pkg",
    sha256 = "8f9ee2dc10c1ae514ee599a8b42ed99fa262b757058f65ad3c384289ff70c4b8",
    urls = [
        "https://github.com/bazelbuild/rules_pkg/releases/download/{}/rules_pkg-{}.tar.gz".format(RULES_PKG_VERSION, RULES_PKG_VERSION),
        "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/{}/rules_pkg-{}.tar.gz".format(RULES_PKG_VERSION, RULES_PKG_VERSION),
    ],
)

load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("@rules_pkg//:rpm.bzl", "pkg_rpm")

rules_pkg_dependencies()
