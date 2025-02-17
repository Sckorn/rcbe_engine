cc_binary(
    name = "check_tga",
    srcs = select({
        "@bazel_tools//src/conditions:windows": ["src/winmain.cpp"],
        "//conditions:default": ["src/linuxmain.cpp"], #TODO: @sckorn @radameon we prolly should handle linux case explicitly
    }), 
    data = [
        "@awesomeface_texture//file",
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": [
            "/D_UNICODE", 
            "/DUNICODE", 
            "/Zc:wchar_t",
        ],
        "//conditions:default": [],
    }),
    linkopts = select({
        "@bazel_tools//src/conditions:windows": [],
        "//conditions:default": [
            "-lpthread",
            "-lX11",
            "-lXext",
        ],
    }),
    deps = [
        "//window_manager:rdmn_engine_window_manager",
        "//datamodel:rdmn_engine_datamodel_visual",
        "//common:rdmn_engine_common_trivial_logger",
        "//parsers:rdmn_engine_parser_tga",
    ],
)