#ifndef RCBE_ENGINE_PROFILING_UTILS_HPP
#define RCBE_ENGINE_PROFILING_UTILS_HPP

#include <chrono>
#include <concepts>
#include <ostream>
#include <type_traits>
#include <utility>

namespace rcbe::utils {
template <typename T>
concept OutputStreamType = std::derived_from<T, std::ostream>;

template <typename T>
concept ObjectByValue = !std::is_fundamental_v<T> && !std::is_pointer_v<T>;

template <OutputStreamType S, typename F, typename... Args>
decltype(auto) profiled_call(S &s, F &&f, Args &&...a) {
    auto start = std::chrono::steady_clock::now();
    auto &&res = f(std::forward<Args>(a)...);
    auto end = std::chrono::steady_clock::now();
    s << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
    return res;
}

template <OutputStreamType S, ObjectByValue C, typename F, typename... Args>
decltype(auto) profiled_call(S &s, C c, F f, Args &&...a) {
    auto start = std::chrono::steady_clock::now();
    auto &&res = (c.*f)(std::forward<Args>(a)...);
    auto end = std::chrono::steady_clock::now();
    s << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
    return res;
}

template <OutputStreamType S, typename F>
decltype(auto) profiled_call(S &s, F &&f) {
    auto start = std::chrono::steady_clock::now();
    auto &&res = f();
    auto end = std::chrono::steady_clock::now();
    s << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
    return res;
}
}// namespace rcbe::utils

#endif//RCBE_ENGINE_PROFILING_UTILS_HPP
