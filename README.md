# RDMN (RADAMEON :copyright:) engine
Video game framework (to-be), who's main purpose is to be a training pet-project, with possibility of becoming something more.

## Architectures supported

Supported and to-be supported architectures are listed in the table below.

|Arch|Support status|
|:------|----------|
|x86-64|:heavy_check_mark:|
|aarch64|WIP|
|RV64|TBD|

## OS support

Supported and to-be supported OS are listed in the table below.

|OS|Support status|
|:------|----------|
|Linux (Debian based)|:heavy_check_mark:|
|Windows|WIP|
|MacOS|TBD|

## Compiler and rasterizer support

Due to project being in pre-alpha state.  And in order to be as close to the bleeding edge as possible, I try to update toolchains as often as possible.
So as of now there is little to none backward compiler support.

> [!IMPORTANT]
> Currently project uses C++20, and we will move to C++23 ASAP.

Compiler and rasterizer support table can be found below.

|Compiler\Rasterizer|Vulkan|OpenGL|
|:-------|----------|---|
|GCC 13.2|:heavy_check_mark:|:heavy_check_mark:|
|Clang 16|:heavy_check_mark:|:heavy_check_mark:|
|MSVC 17.0.1|WIP|:x:|

> [!WARNING]
> OpenGL rasterizer is a legacy feature, and will be deprecated soon.

## Building project from source

RDMN engine uses [Google Bazel](https://bazel.build/) build system, to build the project. 

> [!IMPORTANT]
> Current Bazel version used is 7.0.0

The project is built inside docker container, specified in this [Dockerfile](docker/dockerfiles/build.docker). This is the default, and only supported build from source scenario.
The docker image for build can also be obtained from [Dockerhub](https://hub.docker.com/r/radameon/build/tags).
And can be pulled via the following command.
```
docker pull radameon/build
```

Currently it holds the following tags
|Tag name| Descriptiom|
|:-|:-|
|gcc-13|Build image with GCC 13, Clang 16 and Bazel 7.0.0|
|clang-16|Build image with GCC 13, Clang 16 and Bazel 7.0.0|
|gcc-12|Build image with GCC 12, Clang 14 and Bazel 5.2.0|
|clang-14|Build image with GCC 12, Clang 14 and Bazel 5.2.0|

### Configuring bazel build

Right now you can build the project with two different options for rasterizers.

> [!NOTE]
> All bazel commands are performed from within the repo root.

The below command builds the project with (Vulkan)[https://www.vulkan.org/] configured as rasterizer backend

```
bazel build --//meta_package:rdmn_rasterizer_back=vulkan ...
```

Possible values for `meta_package:rdmn_rasterizer_back` option are:
1. `vulkan`
2. `opengl`
3. `directx`

> [!CAUTION]
> `directx` value is added, but is Work-In-Progress. Choosing this value will result in `static_assert`s firing throughout the code.

## Binary releases

From the beginning we were providing binary releases. With later ones being packaged as deb packages.
These packages can be obtained from (releases)[https://github.com/Sckorn/rcbe_engine/releases] page.

> [!CAUTION]
> Since we are in the pre-alpha stage, packages **DO NOT** have a stable API
> And may hold breaking changes, between different versions.

## Maintainers

The single maintainer is @Sckorn (me).
If you want to contact me, you can do so through the info provided in my profile page.
