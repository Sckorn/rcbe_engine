#ifndef RCBE_ENGINE_TIME_TRAITS_HPP
#define RCBE_ENGINE_TIME_TRAITS_HPP

#include <type_traits>
#include <chrono>

namespace rcbe::utility {
template <typename Duration>
struct IsDurationType : public std::false_type {};

template <>
struct IsDurationType<std::chrono::hours> : public std::true_type {};

template <>
struct IsDurationType<std::chrono::minutes> : public std::true_type {};

template <>
struct IsDurationType<std::chrono::seconds> : public std::true_type {};

template <>
struct IsDurationType<std::chrono::milliseconds> : public std::true_type {};

template <>
struct IsDurationType<std::chrono::microseconds> : public std::true_type {};

template <>
struct IsDurationType<std::chrono::nanoseconds> : public std::true_type {};

template <typename Duration>
static constexpr bool duration_type_v = IsDurationType<Duration>::value;
}

#endif //RCBE_ENGINE_TIME_TRAITS_HPP
