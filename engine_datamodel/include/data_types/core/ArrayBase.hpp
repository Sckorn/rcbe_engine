#ifndef RCBE_ARRAYBASE_HPP
#define RCBE_ARRAYBASE_HPP

#include <array>

namespace rcbe::core
{
template <typename ValueType, size_t dim>
class ArrayBase
{
public:
    static constexpr size_t dimension = dim;
public:
    using value_type = ValueType;

public:
    ArrayBase()
    {
        for (size_t i = 0; i < dim; ++i)
        {
            _word[i] = 0;
        }
    };

    template <typename... Ts, typename = std::enable_if_t< (sizeof...(Ts) == dim) > >
    ArrayBase(Ts&&... args)
    :
    _word{ { args... } }
    {
        
    }

    // TODO: in a comment below
    // provide two versions one for trivially copyable types (this), and one for custom type (returning const reference)
    // open 
    const EngineScalar &m(const size_t index) const
    {
        rangeCheck(index);
        return _word[index];
    }

    const EngineScalar &operator[](const size_t index) const
    {
        rangeCheck(index);
        return _word[index];
    }

    EngineScalar &at(size_t index)
    {
        rangeCheck(index);
        return _word[index];
    }

    EngineScalar &operator[](size_t index)
    {
        rangeCheck(index);
        return _word[index];
    }

private:

    void rangeCheck(const size_t index)
    {
        if (index > _word.size())
            throw std::runtime_error("ArrayBase out of range access!");
    }

    std::array<value_type, dimension> _word;
};
}

#endif