#ifndef RCBE_ENGINE_BASE_SERIALIZER_HPP
#define RCBE_ENGINE_BASE_SERIALIZER_HPP

#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace rcbe::binary {
template <typename Buffer, typename ActualType>
void to_binary(Buffer &c, const ActualType &a) {
    throw std::runtime_error("Please provide to_binary implementation for the type !");
}

template <typename Buffer, typename ActualType>
void from_binary(const Buffer &c, ActualType& a) {
    throw std::runtime_error("Please provide from_binary implementation for the type " + std::string(typeid(ActualType).name()) +  " !");
}

class BinaryBuffer;

template <>
void to_binary<BinaryBuffer, int>(BinaryBuffer &b, const int &a);

template <>
void from_binary<BinaryBuffer, int>(const BinaryBuffer &b, int &a);

template <>
void to_binary<BinaryBuffer, short>(BinaryBuffer &b, const short &a);

template <>
void from_binary<BinaryBuffer, short>(const BinaryBuffer &b, short &a);

template <>
void to_binary<BinaryBuffer, long>(BinaryBuffer &b, const long &a);

template <>
void from_binary<BinaryBuffer, long>(const BinaryBuffer &b, long &a);

template <>
void to_binary<BinaryBuffer, float>(BinaryBuffer &b, const float &a);

template <>
void from_binary<BinaryBuffer, float>(const BinaryBuffer &b, float &a);

template <>
void to_binary<BinaryBuffer, double>(BinaryBuffer &b, const double &a);

template <>
void from_binary<BinaryBuffer, double>(const BinaryBuffer &b, double &a);

template <>
void to_binary<BinaryBuffer, long double>(BinaryBuffer &b, const long double &a);

template <>
void from_binary<BinaryBuffer, long double>(const BinaryBuffer &b, long double &a);

template <>
void to_binary<BinaryBuffer, size_t>(BinaryBuffer &b, const size_t &a);

template <>
void from_binary<BinaryBuffer, size_t>(const BinaryBuffer &b, size_t &a);

template <>
void to_binary<BinaryBuffer, char>(BinaryBuffer &b, const char &a);

template <>
void from_binary<BinaryBuffer, char>(const BinaryBuffer &b, char &a);

template <>
void to_binary<BinaryBuffer, uint32_t>(BinaryBuffer &b, const uint32_t &a);

template <>
void from_binary<BinaryBuffer, uint32_t>(const BinaryBuffer &b, uint32_t &a);

template <>
void to_binary<BinaryBuffer, uint16_t>(BinaryBuffer &b, const uint16_t &a);

template <>
void from_binary<BinaryBuffer, uint16_t>(const BinaryBuffer &b, uint16_t &a);

}

#endif //RCBE_ENGINE_BASE_SERIALIZER_HPP
