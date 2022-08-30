#ifndef RDMN_CONCEPTS_HPP
#define RDMN_CONCEPTS_HPP

#include <concepts>

namespace rdmn::core {
template <typename T>
concept NumberValue = std::integral<std::remove_cvref_t<T>> 
    || std::floating_point<std::remove_cvref_t<T>>;
}

#endif // RDMN_CONCEPTS_HPP