#include <rcbe-engine/binary/BinaryBuffer.hpp>
#include <rcbe-engine/binary/base_serializer.hpp>

namespace rcbe::binary {
template <>
R_PUBLIC_API void to_binary<BinaryBuffer, int>(BinaryBuffer &b, const int &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, int>(const BinaryBuffer &b, int &a) {
    a = b.at(0, sizeof(int)).get<int>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, short>(BinaryBuffer &b, const short &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, short>(const BinaryBuffer &b, short &a) {
    a = b.at(0, sizeof(short)).get<short>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, long>(BinaryBuffer &b, const long &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, long>(const BinaryBuffer &b, long &a) {
    a = b.at(0, sizeof(long)).get<long>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, float>(BinaryBuffer &b, const float &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, float>(const BinaryBuffer &b, float &a) {
    a = b.at(0, sizeof(float)).get<float>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, double>(BinaryBuffer &b, const double &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, double>(const BinaryBuffer &b, double &a) {
    a = b.at(0, sizeof(double)).get<double>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, long double>(BinaryBuffer &b, const long double &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, long double>(const BinaryBuffer &b, long double &a) {
    a = b.at(0, sizeof(long double)).get<long double>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, size_t>(BinaryBuffer &b, const size_t &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, size_t>(const BinaryBuffer &b, size_t &a) {
    a = b.at(0, sizeof(size_t)).get<size_t>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, char>(BinaryBuffer &b, const char &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, char>(const BinaryBuffer &b, char &a) {
    a = b.at(0, sizeof(char)).get<char>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, uint32_t>(BinaryBuffer &b, const uint32_t &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, uint32_t>(const BinaryBuffer &b, uint32_t &a) {
    a = b.at(0, sizeof(uint32_t)).get<uint32_t>();
}

template <>
R_PUBLIC_API void to_binary<BinaryBuffer, uint16_t>(BinaryBuffer &b, const uint16_t &a) {
    b = BinaryBuffer(a);
}

template <>
R_PUBLIC_API void from_binary<BinaryBuffer, uint16_t>(const BinaryBuffer &b, uint16_t &a) {
    a = b.at(0, sizeof(uint16_t)).get<uint16_t>();
}
}// namespace rcbe::binary