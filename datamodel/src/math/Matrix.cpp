#include <rcbe-engine/datamodel/math/Matrix.hpp>

namespace nlohmann {
R_PUBLIC_API void adl_serializer<rcbe::math::Matrix3x3>::to_json(
    nlohmann::json &j,
    const rcbe::math::Matrix3x3 &value) {
    j = {
        {"rows", value.ROWS},
        {"columns", value.COLUMNS},
        {"matrix", value.data()}};
}

R_PUBLIC_API void adl_serializer<rcbe::math::Matrix3x3>::from_json(const nlohmann::json &j, rcbe::math::Matrix3x3 &value) {
    auto data = j.at("matrix").get<rcbe::math::Matrix3x3::StorageType>();
    auto rows = j.at("rows").get<rcbe::core::EngineIntergral>();
    auto cols = j.at("columns").get<rcbe::core::EngineIntergral>();
    if (rows != 3 || cols != 3)
        throw std::runtime_error("Wrong deserialized Matrix sizes");

    value = rcbe::math::Matrix3x3(data.begin(), data.end());
}
}// namespace nlohmann

R_PUBLIC_API rcbe::math::Matrix3x3 operator*(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Matrix3x3 &rhs) {
    std::vector<rcbe::math::Matrix3x3::ValueType> interm;
    interm.reserve(lhs.ROWS * lhs.COLUMNS);
    for (size_t i = 0; i < lhs.ROWS * lhs.COLUMNS; ++i) {
        const size_t row_index = i / lhs.ROWS;
        const size_t sub_index = i % lhs.COLUMNS;

        const auto r_column = rhs.getColumn(sub_index);

        interm.push_back(
            lhs.at(row_index, 0) * r_column.x() +
            lhs.at(row_index, 1) * r_column.y() +
            lhs.at(row_index, 2) * r_column.z());
    }

    return rcbe::math::Matrix3x3 {interm};
}

R_PUBLIC_API rcbe::math::Matrix3x3 operator+(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Matrix3x3 &rhs) {
    rcbe::math::Matrix3x3::StorageType intermideate_storage {};

    for (size_t i = 0; i < lhs.ROWS; ++i) {
        for (size_t j = 0; j < lhs.COLUMNS; ++j) {
            intermideate_storage[i * lhs.ROWS + j] = lhs.at(i, j) + rhs.at(i, j);
        }
    }

    return rcbe::math::Matrix3x3(std::make_move_iterator(intermideate_storage.begin()), std::make_move_iterator(intermideate_storage.end()));
}

R_PUBLIC_API rcbe::math::Vector3d operator*(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Vector3d &rhs) {
    rcbe::math::Vector3d::StorageType intermediate_storage {};
    for (size_t i = 0; i < lhs.ROWS; ++i) {
        intermediate_storage[i] = lhs.at(i, 0) * rhs.x() + lhs.at(i, 1) * rhs.y() + lhs.at(i, 2) * rhs.z();
    }

    return rcbe::math::Vector3d(intermediate_storage);
}

R_PUBLIC_API rcbe::math::Matrix4x4 operator*(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Matrix4x4 &rhs) {
    std::vector<rcbe::math::Matrix3x3::ValueType> interm;
    interm.reserve(lhs.ROWS * lhs.COLUMNS);
    for (size_t i = 0; i < lhs.ROWS * lhs.COLUMNS; ++i) {
        const size_t row_index = i / lhs.ROWS;
        const size_t sub_index = i % lhs.COLUMNS;

        const auto r_column = rhs.getColumn(sub_index);

        interm.push_back(
            lhs.at(row_index, 0) * r_column.x() +
            lhs.at(row_index, 1) * r_column.y() +
            lhs.at(row_index, 2) * r_column.z() +
            lhs.at(row_index, 3) * r_column.w());
    }

    return rcbe::math::Matrix4x4 {interm};
}

R_PUBLIC_API rcbe::math::Matrix4x4 operator+(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Matrix4x4 &rhs) {
    rcbe::math::Matrix4x4::StorageType intermideate_storage {};

    for (size_t i = 0; i < lhs.ROWS; ++i) {
        for (size_t j = 0; j < lhs.COLUMNS; ++j) {
            intermideate_storage[i * lhs.ROWS + j] = lhs.at(i, j) + rhs.at(i, j);
        }
    }

    return rcbe::math::Matrix4x4(std::make_move_iterator(intermideate_storage.begin()), std::make_move_iterator(intermideate_storage.end()));
}

R_PUBLIC_API rcbe::math::Vector4d operator*(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Vector4d &rhs) {
    rcbe::math::Vector4d::StorageType intermediate_storage {};
    for (size_t i = 0; i < lhs.ROWS; ++i) {
        intermediate_storage[i] = lhs.at(i, 0) * rhs.x() + lhs.at(i, 1) * rhs.y() + lhs.at(i, 2) * rhs.z() + lhs.at(i, 3) * rhs.w();
    }

    return rcbe::math::Vector4d(intermediate_storage);
}