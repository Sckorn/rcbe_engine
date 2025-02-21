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
        deps: All non-TF dependencies
        tf_deps: All TF depenedencies
        copts: C options
        compatible_with: List of environments target can be built for
        testonly: If library is only for testing
        alwayslink: If symbols should be exported
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