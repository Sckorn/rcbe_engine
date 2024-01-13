#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::binary {
template <>
void from_binary(const BinaryBuffer &b, rcbe::math::Vector3f &v) {
    auto x = b.at(0, sizeof(float)).get<float>();
    auto y = b.at(sizeof(float), sizeof(float)).get<float>();
    auto z = b.at(sizeof(float) * 2, sizeof(float)).get<float>();

    v = {x, y, z};
}

template <>
void to_binary(BinaryBuffer &b, const rcbe::math::Vector3f &v) {
    b = {v.x(), v.y(), v.z()};
}

template <>
void from_binary(const BinaryBuffer &b, rcbe::math::Vector2f &v) {
    const auto x = b.at(0, sizeof(float)).get<float>();
    const auto y = b.at(sizeof(float), sizeof(float)).get<float>();

    v = {x, y};
}

template <>
void to_binary(BinaryBuffer &b, const rcbe::math::Vector2f &v) {
    b = {v.x(), v.y()};
}
}