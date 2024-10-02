ENGINE_VERSION_MAJOR = "0"
ENGINE_VERSION_MINOR = "0"
ENGINE_VERSION_PATCH = "4"
ENGINE_VERSION_FULL = "{}.{}.{}".format(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH)
TARGET_ARCH = "amd64"
BOOST_TARGET_VERSION = "1.74"
PROJECT_URL = "https://github.com/Sckorn/rcbe_engine"
PROJECT_MAINTAINER = "Daniil Kireev <sckorn@gmail.com>"

LOGGING_LIB = select({
    "@bazel_tools//src/conditions:windows": ["@com_github_google_glog//:glog"],
    "//conditions:default": ["@boost//:log"],
})

LOGGING_LIB_DEFINE = select({
    "@bazel_tools//src/conditions:windows": ["R_USE_GLOG"],
    "//conditions:default": ["R_USE_BOOST_LOG"],
})
