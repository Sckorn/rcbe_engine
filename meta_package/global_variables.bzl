ENGINE_VERSION_MAJOR = "0"
ENGINE_VERSION_MINOR = "1"
ENGINE_VERSION_PATCH = "0"
ENGINE_VERSION_FULL = "{}.{}.{}".format(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH)
TARGET_ARCH = "amd64"
PROJECT_URL = "https://github.com/Sckorn/rcbe_engine"
PROJECT_MAINTAINER = "Daniil Kireev <sckorn@gmail.com>"
ENGINE_BUILD_TARGET_PREFIX = "rdmn_engine"

WINDOWS_BUILD_DEFINES = [ # use it later # TODO: utilize it instead of a separate target with include file
    "R_PUBLIC_API=__declspec(dllexport)",
]

PUBLIC_API_PREFIX = select({ # use it later
    "@bazel_tools//src/conditions:windows": WINDOWS_BUILD_DEFINES,
    "//conditions:default": ["R_PUBLIC_API="],
})

SHARED_LIB_SUFFIX = "shared"
