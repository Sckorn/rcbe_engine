#ifndef RCBE_ENGINE_PROFILING_UTILS_HPP
#define RCBE_ENGINE_PROFILING_UTILS_HPP

#include <chrono>
#include <utility>
#include <ostream>

namespace rcbe::utils {
// TODO: use concepts
template <typename S, typename F, typename ... Args>
decltype(auto) profiled_call(S& s, F f, Args&&... a) {
    auto start = std::chrono::steady_clock::now();
    auto&& res = f(std::forward<Args>(a)...);
    auto end = std::chrono::steady_clock::now();
    s << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    return res;
}

template <typename S, typename C, typename F, typename ... Args>
decltype(auto) profiled_call(S& s, C&& c, F f, Args&&... a) {
    auto start = std::chrono::steady_clock::now();
    auto&& res = (c->*f)(std::forward<Args>(a)...);
    auto end = std::chrono::steady_clock::now();
    s << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    return res;
}

template <typename S, typename F, typename ... Args>
void profiled_call(S& s, F f, Args&&... a) {
    auto start = std::chrono::steady_clock::now();
    auto&& res = f(std::forward<Args>(a)...);
    auto end = std::chrono::steady_clock::now();
    s << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    return res;
}

template <typename S, typename F>
decltype(auto) profiled_call(S& s, F f) {
    auto start = std::chrono::steady_clock::now();
    auto&& res = f();
    auto end = std::chrono::steady_clock::now();
    s << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    return res;
}
}

#endif //RCBE_ENGINE_PROFILING_UTILS_HPP
