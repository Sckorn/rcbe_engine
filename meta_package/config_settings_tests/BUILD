load("@rules_wix//:rules.bzl", "pkg_msi")

cc_binary(
    name = "test_config_settings",
    srcs = select({
        "//meta_package:rdmn_gl_rasterizer_backend": ["glmain.cpp"],
        "//meta_package:rdmn_vk_rasterizer_backend": ["vkmain.cpp"],
    }),
    defines = select({
        "//meta_package:rdmn_vulkan_windows": ["R_VULKAN_WND"],
        "//meta_package:rdmn_vulkan_linux": ["R_VULKAN_LNX"],
        "//conditions:default": [],
    }),
    deps = ["//meta_package:rdmn_engine_config"],
)

pkg_msi(
  name = "test_config_settings.msi",
  src = "packaging/test_config_settings.wxs",
  deps = [
    ":test_config_settings",
  ],
  arch = "x64",
  exts = [
    "WixUiExtension",
  ],
)
