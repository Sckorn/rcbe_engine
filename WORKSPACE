workspace(name = "rcbe_engine")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

skylib_version = "0.7.0"
http_archive(
    name = "bazel_skylib",
    type = "tar.gz",
    url = "https://github.com/bazelbuild/bazel-skylib/releases/download/{}/bazel-skylib.{}.tar.gz".format (skylib_version, skylib_version),
    sha256 = "2ef429f5d7ce7111263289644d233707dba35e39696377ebab8b0bc701f7818e",
)

# External deps

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "6d6fd834281cb8f8e758dd9ad76df86304bf1869",
    remote = "https://github.com/nelhage/rules_boost",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

http_archive(
    name = "CGAL",
    url = "http://localhost:8080/static/CGAL-4.13.1.tar.gz",
    build_file = "@//thirdparty:CGAL.BUILD",
    strip_prefix = "CGAL-4.13.1",
    #sha256 = "112c4e1ce8969a3c29e77ee8d58c2b9e4b3cbab5234af187207ab8ca7ddd2575",
)

# Group the sources of the library so that CMake rule have access to it
#all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

# Rule repository
#http_archive(
#   name = "rules_foreign_cc",
#   strip_prefix = "rules_foreign_cc-master",
#   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
#)

#load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

#load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

#rules_foreign_cc_dependencies([
#    "//:built_cmake_toolchain",
#    "//:built_ninja_toolchain_osx",
#    "//:built_ninja_toolchain_linux",
#])

# Call this function from the WORKSPACE file to initialize rules_foreign_cc
#  dependencies and let neccesary code generation happen
#  (Code generation is needed to support different variants of the C++ Starlark API.).
#
#  Args:
#    native_tools_toolchains: pass the toolchains for toolchain types
#      '@rules_foreign_cc//tools/build_defs:cmake_toolchain' and
#      '@rules_foreign_cc//tools/build_defs:ninja_toolchain' with the needed platform constraints.
#      If you do not pass anything, registered default toolchains will be selected (see below).
#  
#    register_default_tools: if True, the cmake and ninja toolchains, calling corresponding
#      preinstalled binaries by name (cmake, ninja) will be registered after
#      'native_tools_toolchains' without any platform constraints.
#      The default is True.
#rules_foreign_cc_dependencies(["//:my_cmake_toolchain", "//:my_ninja_toolchain"])

# Eigen source code repository
#http_archive(
    #name = "CGAL",
    #build_file_content = all_content,#"//thirdparty/CGAL.BUILD",
    #build_file = "@//thirdparty:CGAL.BUILD",
    #strip_prefix = "CGAL-4.13.1",
    #urls = ["https://github.com/CGAL/cgal/archive/releases/CGAL-4.13.1.tar.gz"],
    #sha256 = "f252c5211d87bf677bb4075f748380201979af405dff7bbe51a7de565144c5c5",
#)