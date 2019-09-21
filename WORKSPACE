workspace(name = "rcbe_engine")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# bazel-skylib 0.8.0 released 2019.03.20 (https://github.com/bazelbuild/bazel-skylib/releases/tag/0.8.0)
skylib_version = "0.8.0"
http_archive(
    name = "bazel_skylib",
    type = "tar.gz",
    url = "https://github.com/bazelbuild/bazel-skylib/releases/download/{}/bazel-skylib.{}.tar.gz".format (skylib_version, skylib_version),
    sha256 = "2ef429f5d7ce7111263289644d233707dba35e39696377ebab8b0bc701f7818e",
)

# External deps

http_archive(
    name = "boost",
    build_file = "@//thirdparty/boost:boost.BUILD",
    url = "http://localhost:8080/static/boost_1_71_0_gcc9.tar.gz",
    sha256 = "8e4bd2759ac539a6d1c4750a8b575f20bc544d77678770df65aaba5e9c5db9d9",
    strip_prefix = "boost_1_71_0",
)

new_local_repository(
    name = "opengl",
    build_file = "thirdparty/opengl.BUILD",
    path = "/usr/",
)

new_local_repository(
    name = "gtkmm",
    build_file = "thirdparty/gtkmm.BUILD",
    path = "/usr/",
)

new_local_repository(
    name = "gdkmm",
    build_file = "thirdparty/gdkmm.BUILD",
    path = "/usr/",
)

new_local_repository(
    name = "glibmm",
    build_file = "thirdparty/glibmm.BUILD",
    path = "/usr/",
)

new_local_repository(
    name = "glib",
    build_file = "thirdparty/glib.BUILD",
    path = "/usr/",
)

new_local_repository(
    name = "sigpp",
    build_file = "thirdparty/sigpp.BUILD",
    path = "/usr/",
)

#new_local_repository(
#    name = "gcc_lib",
#    build_file = "thirdparty/gcc_lib.BUILD",
#    path = "/",
#)


# Group the sources of the library so that CMake rule have access to it
#all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

# Rule repository
#http_archive(
#   name = "rules_foreign_cc",
#   strip_prefix = "rules_foreign_cc-master",
#   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
#)

#load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

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
#    name = "CGAL",
#    build_file_content = "//thirdparty/CGAL.BUILD",
#    strip_prefix = "CGAL-4.13.1",
#    urls = ["https://github.com/CGAL/cgal/archive/releases/CGAL-4.13.1.tar.gz"],
#)