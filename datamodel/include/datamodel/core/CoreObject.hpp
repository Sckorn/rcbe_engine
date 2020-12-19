#ifndef RCBE_COREOBJECT_HPP
#define RCBE_COREOBJECT_HPP

#include <functional>
#include <memory>
#include <chrono>

namespace rcbe::core
{
class CoreObject
{
public:

    template <typename T>
    CoreObject(const T &obj)
    :
    impl_(std::make_shared<CObjectImpl<T>>(obj))
    {}

    template <typename T>
    const T &as() const
    {
        return std::static_pointer_cast<CObjectImpl<T>>(impl_)->get();
    }

    [[nodiscard]] size_t hash() const
    {
        return impl_->hash();
    }

private:

    struct CObjectImplInterface
    {
        virtual size_t hash() const = 0;
        virtual size_t id() const noexcept = 0;
    };

    template <typename T>
    struct CObjectImpl : CObjectImplInterface
    {
    public:
        CObjectImpl(const T &o)
        :
        value_ { o }
        , id_ { std::hash<size_t>()(std::chrono::steady_clock::now()) }
        {}

        [[nodiscard]] size_t hash() const override
        {
            return std::hash<size_t>()(id_);
        }

        [[nodiscard]] size_t id() const noexcept override
        {
            return id_;
        }

    private:
        T value_;
        const size_t id_;
    };

    std::shared_ptr<CObjectImplInterface> impl_;
};
}

#endif