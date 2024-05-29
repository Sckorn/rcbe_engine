#ifndef RDMN_PUBLIC_API_HPP
#define RDMN_PUBLIC_API_HPP

#ifdef _WIN32

#ifdef RDMN_BUILD_SHARED_LIBS

#define R_PUBLIC_API __declspec(dllexport)

#else

#define R_PUBLIC_API __declspec(dllimport)

#endif

#elif defined(__linux__)

#ifndef RDMN_BUILD_SHARED_LIBS

static_assert(false, "Linux builds only support shared libs build, as of now!");

#endif

#define R_PUBLIC_API

#endif

#endif // #ifdef RDMN_PUBLIC_API_HPP