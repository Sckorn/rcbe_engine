filegroup(
    name = "gltf_tests_archive",
    srcs = glob([
        "**",
    ], exclude = ["BUILD.bazel", "WORKSPACE"]),
    visibility = ["//visibility:public"],
)

filegroup(
    name = "gltf_scene_files",
    srcs = [
        "viking_room/scene.gltf",
        "viking_room/scene.bin",
    ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "gltf_tex_files",
    srcs = glob([
        "viking_room/textures/*",
    ]),
    visibility = ["//visibility:public"],
)
