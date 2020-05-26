#include <data_types/geometry/geometry.hpp>

std::istream &operator>>(std::istream &is, rcbe::geometry::Triangle &tri)
{
    int sep;
    is >> tri[0] >> tri[1] >> tri[2] >> sep;
    return is;
}

namespace nlohmann
{
void adl_serializer<rcbe::geometry::Triangle>::to_json(nlohmann::json &j, const rcbe::geometry::Triangle &t)
{
    j = {
        {"indices", {t[0], t[1], t[2]}}
    };
}

void adl_serializer<rcbe::geometry::Triangle>::from_json(const nlohmann::json &j, rcbe::geometry::Triangle &t)
{
    auto indices = j.at("indices").get<std::vector<rcbe::geometry::Triangle::value_type>>();
    t = {indices[0], indices[1], indices[2]};
}
}