workspace(name = "rcbe_engine")

# bazel-skylib 0.8.0 released 2019.03.20 (https://github.com/bazelbuild/bazel-skylib/releases/tag/0.8.0)
skylib_version = "0.8.0"
http_archive(
    name = "bazel_skylib",
    type = "tar.gz",
    url = "https://github.com/bazelbuild/bazel-skylib/releases/download/{}/bazel-skylib.{}.tar.gz".format (skylib_version, skylib_version),
    sha256 = "2ef429f5d7ce7111263289644d233707dba35e39696377ebab8b0bc701f7818e",
)

#########################################################
# Docker rules (language specifix rules added below)
#########################################################
#http_archive(
#    name = "io_bazel_rules_docker",
#    sha256 = "aed1c249d4ec8f703edddf35cbe9dfaca0b5f5ea6e4cd9e83e99f3b0d1136c3d",
#    strip_prefix = "rules_docker-0.7.0",
#    url = "bazel/rules_docker-0.7.0.tar.gz",
#)

#load(
#    "@io_bazel_rules_docker//repositories:repositories.bzl",
#    container_repositories = "repositories",
#)

#container_repositories()

# Uncomment when it is clear what to do with rules

#########################################################
# Third-party dependencies for planner cxx and related packages
#########################################################
load("//thirdparty:thirdparty_dependencies.bzl", "thirdparty_dependecies")

#thirdparty_dependecies()

#########################################################
# Test resources
#########################################################
#load("//integration_tests_data:integration_tests_data.bzl", "pull_integration_tests_data")

#pull_integration_tests_data()

#########################################################
# Python rules and pip dependecies
#########################################################
#http_archive(
#    name = "io_bazel_rules_python",
#    sha256 = "e06625b0947abed12f03d0f5a7007544a81d70d056fd2fb2c219fdb7048536d6",
#    strip_prefix = "rules_python-fdbb17a4118a1728d19e638a5291b4c4266ea5b8",
#    url = "bazelbuild/rules_python/archive/fdbb17a4118a1728d19e638a5291b4c4266ea5b8.zip",
#)

# Only needed for PIP support:
#load("@io_bazel_rules_python//python:pip.bzl", "pip_import", "pip_repositories")

#pip_repositories()

#pip_import(
#    name = "pip_deps",
#    requirements = "//thirdparty:requirements.txt",
#)

# Load the pip_install symbol for my_deps, and create the dependencies'
# repositories.
#load("@pip_deps//:requirements.bzl", "pip_install")

#pip_install()

#########################################################
# GRPC and protobuf
#########################################################
#http_archive(
#    name = "com_github_grpc_grpc",
#    sha256 = "1bf082fb3016154d3f806da8eb5876caf05743da4b2e8130fadd000df74b5bb6",
#    strip_prefix = "grpc-1.21.1",
#    urls = [
         #"grpc/grpc/archive/v1.21.1.tar.gz",
    #],
#)

#load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

#grpc_deps()

#pip_import(
#    name = "grpc_python_dependencies",
#    requirements = "@com_github_grpc_grpc//:requirements.bazel.txt",
#)

#load("@com_github_grpc_grpc//bazel:grpc_python_deps.bzl", "grpc_python_deps")

#grpc_python_deps()

#########################################################
# Python grpc support 
#########################################################
#http_archive(
#    name = "org_pubref_rules_protobuf",
#    sha256 = "63f2b20f7d802793235182537c814ddd8d6c977db294ab4dd38b446f1cb43b5d",
#    strip_prefix = "rules_protobuf-606ad297b2cfbf67f4c3f1a8ecc9ee2fe8cbf4e3",
#    url = "CogRobRelease/rules_protobuf/archive/606ad297b2cfbf67f4c3f1a8ecc9ee2fe8cbf4e3.zip",
#)

#load("@org_pubref_rules_protobuf//python:rules.bzl", "py_proto_repositories")

#py_proto_repositories()

#########################################################
# Language specific docker rules
#########################################################

# py docker images
#load(
#    "@io_bazel_rules_docker//python:image.bzl",
#    _py_image_repos = "repositories",
#)

#_py_image_repos()

# nodejs docker images
#load(
#    "@io_bazel_rules_docker//nodejs:image.bzl",
#    _nodejs_image_repos = "repositories",
#)

#_nodejs_image_repos()

# cc docker images
#load(
#    "@io_bazel_rules_docker//cc:image.bzl",
#    _cc_image_repos = "repositories",
#)

#_cc_image_repos()

#load("@io_bazel_rules_docker//container:container.bzl", "container_pull")

#container_pull(
#    name = "rwc_build",
#    registry = "registry.staging.abagy.local:5000",
#   repository = "rwc_build",
#    tag = "ros-expunge",
#)

#container_pull(
#    name = "ros_kinetic_image",
#    registry = "registry.staging.abagy.local:5000",
#    repository = "rwc_ros_nvidia",
#    tag = "latest",
#)
