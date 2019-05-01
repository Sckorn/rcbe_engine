#ifndef RCBE_CORE_TYPES_H
#define RCBE_CORE_TYPES_H

#include <string>

namespace rcbe::data_types::core
{
template <typename ValueType>
class Vector
{
public:

    using value_type = ValueType;

public:
    Vector(ValueType &&x, ValueType &&y, ValueType &&z)
    :
    _x(x)
    , _y(y)
    , _z(z)
    {}
    ~Vector() = default;

    Vector(const Vector &other) = default;
    Vector &operator=(const Vecotr) = default;

    Vector(Vector &&other) = default;
    Vector &operator=(Vector &&other) = default;

private:
    ValueType _x;
    ValueType _y;
    ValueType _z;
};
}

#endif //RCBE_CORE_TYPES_H