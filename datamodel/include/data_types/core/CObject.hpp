#ifndef RCBE_COBJECT_HPP
#define RCBE_COBJECT_HPP

#include <functional>
#include <memory>
#include <chrono>

namespace rcbe::core
{
// TODO: rename into CoreObject
class CObject
{
public:

    template <typename T>
    CObject(const T &obj)
    :
    _impl(std::make_shared<CObjectImpl<T>>(obj))
    {}

    template <typename T>
    const T &as() const
    {
        return std::static_pointer_cast<CObjectImpl<T>>(_impl)->get();
    }

    size_t hash() const
    {
        return _impl->hash();
    }

private:

    struct CObjectImplInterface
    {
        virtual size_t hash() const = 0;
        virtual size_t id() const = 0;
    };

    template <typename T>
    struct CObjectImpl : CObjectImplInterface
    {
    public:
        CObjectImpl(const T &o)
        :
        _value { o }
        , _id { std::hash<size_t>()(std::chrono::steady_clock::now()) }
        {}

        size_t hash() const override
        {
            return std::hash<size_t>()(_id);
        }

        size_t id() const override
        {
            return _id;
        }

    private:
        T _value;
        size_t _id;
    };

    std::shared_ptr<CObjectImplInterface> _impl;
};
}

#endif