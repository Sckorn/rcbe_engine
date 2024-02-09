#ifndef RDMN_ENGINE_CONVINIENCE_HPP
#define RDMN_ENGINE_CONVINIENCE_HPP

#include <string_view>

namespace rdmn::core {
#ifdef __linux

#ifdef __GNUG__
#define R_READABLE_FUNC_NAME __PRETTY_FUNCTION__
#elif __clang__
#define R_READABLE_FUNC_NAME __PRETTY_FUNCTION__
#elif
#define R_READABLE_FUNC_NAME __FUNCTION__
#endif

#elif _WIN32

#ifdef _MSC_VER
#define R_READABLE_FUNC_NAME __FUNCSIG__
#elif
#define R_READABLE_FUNC_NAME __FUNCTION__
#endif

#endif

/// TODO: static_assert doesn't take constexpr constants, so we have to use macro here
#define RASTERIZER_NOT_SET_ERROR_MSG "Rasterizer is not set! Build is malformed!"

}// namespace rdmn::core

#endif//RDMN_ENGINE_CONVINIENCE_HPP
