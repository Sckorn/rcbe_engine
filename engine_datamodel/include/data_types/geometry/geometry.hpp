#ifndef RCBE_GEOMETRY_HPP
#define RCBE_GEOMETRY_HPP

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <iostream>

#include <data_types/config/data_model_config.hpp>
#include <nlohmann/json.hpp>

namespace rcbe::geometry
{
struct Triangle
{
public:
    using value_type = size_t;
public:
    Triangle()
    {
        _storage.fill(0);
    }

    Triangle(const size_t v1, const size_t v2, const size_t v3) noexcept
    :
    _storage( {v1, v2, v3} )
    {}

    Triangle(const Triangle &other) = default;
    Triangle &operator=(const Triangle &other) = default;

    Triangle(Triangle &&other) = default;
    Triangle &operator=(Triangle &&other) = default;

    const value_type &operator[](const size_t index) const noexcept
    {
        return _storage[index];
    }

    value_type &operator[](const size_t index) noexcept
    {
        return _storage[index];
    }

private:
    std::array<size_t, 3> _storage;
};
}

std::istream &operator>>(std::istream &is, rcbe::geometry::Triangle &tri);

namespace nlohmann
{
template <>
struct adl_serializer<rcbe::geometry::Triangle>
{
    static void to_json(nlohmann::json &j, const rcbe::geometry::Triangle &t);
    static void from_json(const nlohmann::json &j, rcbe::geometry::Triangle &t);
};
}

#endif