#include <data_types/math/Matrix.hpp>

namespace nlohmann
{
void adl_serializer<rcbe::math::Matrix3x3> ::to_json(
    nlohmann::json& j, 
    const rcbe::math::Matrix3x3& value
)
{
    j = {
        {"rows", value.rows},
        {"columns", value.columns},
        {"matrix", value.data()}
    };
}

void adl_serializer<rcbe::math::Matrix3x3>:: from_json(const nlohmann::json& j, rcbe::math::Matrix3x3& value)
{
    auto data = j.at("matrix").get<rcbe::math::Matrix3x3::storage_type>();
    auto rows = j.at("rows").get<rcbe::core::EngineIntergral>();
    auto cols = j.at("cols").get<rcbe::core::EngineIntergral>();
    if (rows != 3 || cols != 3)
        throw std::runtime_error("Wrong deserialized Matrix sizes");

    value = rcbe::math::Matrix3x3(data.begin(), data.end());
}
}

rcbe::math::Matrix3x3 operator*(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Matrix3x3 &rhs)
{
    std::vector<rcbe::math::Matrix3x3::value_type> interm;
    interm.reserve(lhs.rows * lhs.columns);
    for (size_t i = 0; i < lhs.rows * lhs.columns; ++i)
    {
        const size_t row_index = i / lhs.rows;
        const size_t sub_index = i % lhs.columns;

        const auto r_column = rhs.getColumn(sub_index);

        interm.push_back(
                lhs.at(row_index, 0) * r_column.x() +
                lhs.at(row_index, 1) * r_column.y() +
                lhs.at(row_index, 2) * r_column.z()
                );
    }

    return rcbe::math::Matrix3x3 {interm};
}

rcbe::math::Matrix3x3 operator+(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Matrix3x3 &rhs)
{
    rcbe::math::Matrix3x3::storage_type intermideate_storage {};

    for (size_t i = 0; i < lhs.rows; ++i)
    {
        for (size_t j = 0; j < lhs.columns; ++j)
        {
            intermideate_storage[i * lhs.rows + j] = lhs.at(i, j) + rhs.at(i, j);
        }
    }

    return rcbe::math::Matrix3x3(std::make_move_iterator(intermideate_storage.begin()), std::make_move_iterator(intermideate_storage.end()));
}

rcbe::math::Vector3d operator*(const rcbe::math::Matrix3x3 &lhs, const rcbe::math::Vector3d &rhs)
{
    rcbe::math::Vector3d::storage_type intermediate_storage {};
    for (size_t i = 0; i < lhs.rows; ++i)
    {
        intermediate_storage[i] = lhs.at(i, 0) * rhs.x() + lhs.at(i, 1) * rhs.y() + lhs.at(i, 2) * rhs.z();
    }

    return rcbe::math::Vector3d(intermediate_storage);
}

rcbe::math::Matrix4x4 operator*(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Matrix4x4 &rhs)
{
    std::vector<rcbe::math::Matrix3x3::value_type> interm;
    interm.reserve(lhs.rows * lhs.columns);
    for (size_t i = 0; i < lhs.rows * lhs.columns; ++i)
    {
        const size_t row_index = i / lhs.rows;
        const size_t sub_index = i % lhs.columns;

        const auto r_column = rhs.getColumn(sub_index);

        interm.push_back(
                lhs.at(row_index, 0) * r_column.x() +
                lhs.at(row_index, 1) * r_column.y() +
                lhs.at(row_index, 2) * r_column.z() +
                lhs.at(row_index, 3) * r_column.w()
                );
    }

    return rcbe::math::Matrix4x4 {interm};
}

rcbe::math::Matrix4x4 operator+(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Matrix4x4 &rhs)
{
    rcbe::math::Matrix4x4::storage_type intermideate_storage {};

    for (size_t i = 0; i < lhs.rows; ++i)
    {
        for (size_t j = 0; j < lhs.columns; ++j)
        {
            intermideate_storage[i * lhs.rows + j] = lhs.at(i, j) + rhs.at(i, j);
        }
    }

    return rcbe::math::Matrix4x4(std::make_move_iterator(intermideate_storage.begin()), std::make_move_iterator(intermideate_storage.end()));
}

rcbe::math::Vector4d operator*(const rcbe::math::Matrix4x4 &lhs, const rcbe::math::Vector4d &rhs)
{
    rcbe::math::Vector4d::storage_type intermediate_storage {};
    for (size_t i = 0; i < lhs.rows; ++i)
    {
        intermediate_storage[i] = lhs.at(i, 0) * rhs.x() + lhs.at(i, 1) * rhs.y() + lhs.at(i, 2) * rhs.z() + lhs.at(i, 3) * rhs.w();
    }

    return rcbe::math::Vector4d(intermediate_storage);
}