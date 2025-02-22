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
        linkopts: Linker options
        copts: C options
        strip_include_prefix: A portion of include path, to strip from virtual include
        visibility: package visibility in terms of Bazel
    """
    defines += select({ 
        "@bazel_tools//src/conditions:windows": ["R_PUBLIC_API=__declspec(dllexport)"],
        "//conditions:default": ["R_PUBLIC_API="],
    })
    linkopts += select({ 
        "@bazel_tools//src/conditions:windows": [],
        "//conditions:default": ["-Wl,-soname,lib{}.so".format(name)],
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