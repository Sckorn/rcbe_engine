#ifndef RCBE_ENGINE_BINARY_FILE_CONCEPTS_HPP
#define RCBE_ENGINE_BINARY_FILE_CONCEPTS_HPP

#include <concepts>
#include <cstddef>

namespace rcbe::core {
template <typename T>
concept BinaryHeader = requires(T &&bh) {
    { T::SIZE };
    { T {} };
};

template <typename T>
concept BinaryChunk = requires(T &&bc) {
    { T::SIZE };
    { T {} };
};
}// namespace rcbe::core

#endif//RCBE_ENGINE_BINARY_FILE_CONCEPTS_HPP
