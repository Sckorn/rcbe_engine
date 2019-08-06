def get_global(path_list) :
    return native.glob(path_list)

include_pattern = get_global(["/usr/include/boost/%s/"]) 

hdrs_patterns = get_global(
    [
        "/usr/include/boost/%s.h",
        "/usr/include/boost/%s_fwd.h",
        "/usr/include/boost/%s.hpp",
        "/usr/include/boost/%s_fwd.hpp",
        "/usr/include/boost/%s/**/*.hpp",
        "/usr/include/boost/%s/**/*.ipp",
        "/usr/include/boost/%s/**/*.h",
    ]
)

srcs_patterns = get_global(
    [
        "/usr/lib/libboost_%s*.so*",
        "/usr/lib/libboost_%s*.so",
    ]
) 

# Building boost results in many warnings for unused values. Downstream users
# won't be interested, so just disable the warning.
default_copts = ["-Wno-unused-value"]

def srcs_list(library_name, exclude):
    return native.glob(
        [p % (library_name,) for p in srcs_patterns],
        exclude = exclude,
    )

def includes_list(library_name):
    return [".", include_pattern % library_name]

def hdr_list(library_name):
    return native.glob([p % (library_name,) for p in hdrs_patterns])

def boost_library(
        name,
        defines = None,
        includes = None,
        hdrs = None,
        srcs = None,
        deps = None,
        copts = None,
        exclude_src = [],
        linkopts = None,
        visibility = ["//visibility:public"]):
    if defines == None:
        defines = []

    if includes == None:
        includes = []

    if hdrs == None:
        hdrs = []
    else:
        hdrs = [hdr for hdr in hdrs]

    if srcs == None:
        srcs = []

    if deps == None:
        deps = []

    if copts == None:
        copts = []

    if linkopts == None:
        linkopts = []

    return native.cc_library(
        name = name,
        visibility = visibility,
        defines = defines,
        includes = includes_list(name) + includes,
        hdrs = hdr_list(name) + hdrs,
        srcs = srcs_list(name, exclude_src) + srcs,
        deps = deps,
        copts = default_copts + copts,
        linkopts = linkopts,
        licenses = ["notice"],
    )
