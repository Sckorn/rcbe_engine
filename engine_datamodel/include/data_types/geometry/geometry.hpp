#ifndef RCBE_GEOMETRY_HPP
#define RCBE_GEOMETRY_HPP

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <iostream>

#include <data_types/data_model_config.hpp>

namespace rcbe::geometry
{
using Triangle = std::array<size_t, 3>;
}

#endif