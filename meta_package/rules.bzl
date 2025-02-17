load(":global_variables.bzl", "ENGINE_VERSION_FULL")
load("//meta_package:global_variables.bzl", "SHARED_LIB_SUFFIX")

def r_cc_library(
        name,
        srcs = [],
        hdrs = [],
        includes = [],
        include_prefix = "",
        defines = [],
        deps = [],
        data = [],
        copts = [],
        linkopts = [],
        visibility = [],
        strip_include_prefix = ""):
    """ A rule to build a RDMN library.

    Args:
        name: Name of library
        srcs: Source files
        hdrs: Headers files
        includes: Include directorires
        include_prefix: A prefix to add to virtual includes
        defines: A list of command lines defines to pass to compiler
        deps: All dependencies
        data: All non code dependencies (configs files, shaders etc.)
        copts: C options
        linkopts: Linker options
        visibility: package visibility in terms of Bazel
        strip_include_prefix: A portion of include path, to strip from virtual include
    """
    defines += select({ 
        "@bazel_tools//src/conditions:windows": ["R_PUBLIC_API=__declspec(dllexport)"],
        "//conditions:default": ["R_PUBLIC_API="],
    })

    native.cc_library(
        name = name,
        srcs = srcs,
        hdrs = hdrs,
        includes = includes,
        include_prefix = include_prefix,
        defines = defines,
        deps = deps,
        data = data,
        copts = copts,
        linkopts = linkopts,
        visibility = visibility,
        strip_include_prefix = strip_include_prefix
    )


def r_cc_shared_library(
    name,
    shared_lib_name,
    deps = [],
    dynamic_deps = [],
    additional_linker_inputs = [],
    user_link_flags = [],
    exports_filter = [],
    visibility = []):
    """ A rule to build a RDMN shared library.

    Args:
        name: Name of library
        shared_lib_name: Name of the rsulting .so or .dll file, in case of .so engine version is appended,
        deps: Static dependencies,
        dynamic_deps: Dynamic dependencies,
        additional_linker_inputs: Linker options,
        user_link_flags: Linker options,
        exports_filter: Static libs, symbols of which are not exported by the shared lib,
        visibility: Package visibility in terms of Bazel,
    """

    shared_lib_name = shared_lib_name + select({
        "@bazel_tools//src/conditions:windows": ".dll",
        "@bazel_tools//src/conditions:linux": ".so.{}".format(ENGINE_VERSION_FULL),
        "//conditions:default": "",
    })

    native.cc_shared_library(
        name = name,
        shared_lib_name = shared_lib_name,
        deps = deps,
        dynamic_deps = dynamic_deps,
        additional_linker_inputs = additional_linker_inputs,
        user_link_flags = user_link_flags,
        exports_filter = exports_filter,
        visibility = visibility
    )