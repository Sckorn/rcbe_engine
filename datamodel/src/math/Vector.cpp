#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace nlohmann
{
void adl_serializer<rcbe::math::Vector3d>::to_json(nlohmann::json &j, const rcbe::math::Vector3d &v)
{
    j = {
        {"dimension", v.DIMENSION},
        {"x", v.x()},
        {"y", v.y()},
        {"z", v.z()},
    };
}

void adl_serializer<rcbe::math::Vector3d>::from_json(const nlohmann::json &j, rcbe::math::Vector3d &v)
{
    v = rcbe::math::Vector3d {
        j.at("x").get<rcbe::core::EngineScalar>(),
        j.at("y").get<rcbe::core::EngineScalar>(),
        j.at("z").get<rcbe::core::EngineScalar>()
    };
}

void adl_serializer<rcbe::math::Vector2d>::to_json(nlohmann::json &j, const rcbe::math::Vector2d &v)
{
    j = {
        {"dimension", v.DIMENSION},
        {"x", v.x()},
        {"y", v.y()},
    };
}

void adl_serializer<rcbe::math::Vector2d>::from_json(const nlohmann::json &j, rcbe::math::Vector2d &v)
{
    v = rcbe::math::Vector2d { j.at("x").get<rcbe::core::EngineScalar>(), j.at("y").get<rcbe::core::EngineScalar>() };
}
}

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
}