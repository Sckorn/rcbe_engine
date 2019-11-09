#include <common/fuzzy_logic/fazzy_logic.hpp>

namespace rcbe::core
{
bool opertator==(double lhs, double rhs)
{
    return fuzzy_equal(lhs, rhs);
}

bool opertator==(float lhs, float rhs)
{
    return fuzzy_equal(lhs, rhs);
}

bool opertator!=(double lhs, double rhs)
{
    return !fuzzy_equal(lhs, rhs);
}

bool opertator!=(float lhs, float rhs)
{
    return !fuzzy_equal(lhs, rhs);
}
}