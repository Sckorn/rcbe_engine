#include <data_types/math/Vector.hpp>

namespace rcbe::math
{
std::ostream &operator<<(std::ostream &os, const Vector3d &vec)
{
    os << "(";
    os << vec.x() << ", " << vec.y() << ", " << vec.z();
    os << ");";
    return os;
}

std::istream &operator>>(std::istream &is, rcbe::math::Vector3d &vec)
{
    rcbe::math::Vector3d::value_type x, y, z;
    is >> x >> y >> z;
    vec = rcbe::math::Vector3d( x, y, z);
    return is;
}
}

namespace nlohmann
{
void adl_serializer<rcbe::math::Vector3d>::to_json(nlohmann::json &j, const rcbe::math::Vector3d &v)
{
    j = {
        {"dimension", v.dimension},
        {"x", v.x()},
        {"y", v.y()},
        {"z", v.z()},
    };
}

void adl_serializer<rcbe::math::Vector3d>::from_json(const nlohmann::json &j, rcbe::math::Vector3d &v)
{
    v = rcbe::math::Vector3d { j.at("x").get<rcbe::core::EngineScalar>(), j.at("y").get<rcbe::core::EngineScalar>(), j.at("z").get<rcbe::core::EngineScalar>() };
}

void adl_serializer<rcbe::math::Vector2d>::to_json(nlohmann::json &j, const rcbe::math::Vector2d &v)
{
    j = {
        {"dimension", v.dimension},
        {"x", v.x()},
        {"y", v.y()},
    };
}

void adl_serializer<rcbe::math::Vector2d>::from_json(const nlohmann::json &j, rcbe::math::Vector2d &v)
{
    v = rcbe::math::Vector2d { j.at("x").get<rcbe::core::EngineScalar>(), j.at("y").get<rcbe::core::EngineScalar>() };
}
}