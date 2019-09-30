#ifndef RCBE_CORE_TYPES_H
#define RCBE_CORE_TYPES_H

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <type_traits>

#include <iostream>

#include <data_model_config.hpp>

namespace rcbe::data_model
{
template <size_t dim, typename Val = EngineScalar>
class ArrayBase
{
public:
    static constexpr size_t DIMENSION = dim;
public:
    using ValueType = Val;

public:
    ArrayBase()
    {
        for (size_t i = 0; i < dim; ++i)
        {
            _word[i] = 0;
        }
    };

    template <typename... Ts, typename = std::enable_if_t<sizeof...(Ts) == dim>>
    ArrayBase(Ts&&... args)
    :
    _word{ { args... } }
    {

    }

    // provide two versions one for trivially copyable types (this), and one for custom type (returning const reference)
    // open 
    EngineScalar m(const size_t index)
    {
        if (index > _word.size())
            throw std::runtime_error("ArrayBase out of range access!");

        return _word[index];
    }

    


private:
    std::array<EngineScalar, dim> _word;
};
}

#endif //RCBE_CORE_TYPES_H