#include <rcbe-engine/datamodel/geometry/triangle_indices.hpp>

std::istream &operator>>(std::istream &is, rcbe::geometry::triangle_indices &tri) {
    int sep;
    is >> tri.indices[0] >> tri.indices[1] >> tri.indices[2] >> sep;
    return is;
}

namespace nlohmann {
void adl_serializer<rcbe::geometry::triangle_indices>::to_json(nlohmann::json &j, const rcbe::geometry::triangle_indices &t) {
    j = {
        {"indices", {t.indices[0], t.indices[1], t.indices[2]}}};
}

void adl_serializer<rcbe::geometry::triangle_indices>::from_json(const nlohmann::json &j, rcbe::geometry::triangle_indices &t) {
    auto indices = j.at("indices").get<std::vector<rcbe::geometry::triangle_indices::ValueType>>();
    t = {indices[0], indices[1], indices[2]};
}
}// namespace nlohmann