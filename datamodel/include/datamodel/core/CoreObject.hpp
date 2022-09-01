#ifndef RCBE_COREOBJECT_HPP
#define RCBE_COREOBJECT_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

namespace rcbe::core
{
class CoreObject {
public:

    template <typename T>
    explicit CoreObject(const T &obj)
    :
    impl_(std::make_shared<CObjectImpl<T>>(obj))
    {}

    template <typename T>
    explicit CoreObject(T &&obj)
    :
    impl_(std::make_shared<CObjectImpl<T>>(std::forward<T>(obj)))
    {}

    template <typename T>
    const T &as() const {
        return std::static_pointer_cast<CObjectImpl<T>>(impl_)->get();
    }

    size_t id() const noexcept {
        return impl_->id();
    }

    [[nodiscard]] size_t hash() const {
        return impl_->hash();
    }

    /// TODO: handle a possibly unsafe case of passing a function or an array here. @sckorn
    /// TODO: think of optimiation using const reference to ptr @sckorn
    template <typename T>
    void addComponent(T &&component) {
        const auto type_ind = std::type_index(typeid(std::decay_t<T>));
        CoreObject comp_co { std::forward<T>(component) };
        components_.insert_or_assign(type_ind, std::make_shared<CoreObject>(std::move(comp_co)));
    }

    /// TODO: handle a possibly unsafe case of passing a function or an array here. @sckorn
    template <typename ComponentType>
    std::shared_ptr<CoreObject> getComponent() const {
        const auto type_ind = std::type_index(typeid(std::decay_t<ComponentType>));
        const auto it = components_.find(type_ind);
        if (it == components_.end())
            return nullptr;

        return it->second;
    }

    [[nodiscard]] bool addSystemTag(std::string &&tag) {
        const auto res = system_tags_.insert(std::move(tag));
        return res.second;
    }

    [[nodiscard]] bool hasSystemTag(const std::string &tag) const {
        return (system_tags_.find(tag) != system_tags_.end());
    }

    /// TODO: handle a possibly unsafe case of passing a function or an array here. @sckorn
    template <typename ComponentType>
    [[nodiscard]] bool hasComponent() const {
        const auto type_ind = std::type_index(typeid(std::decay_t<ComponentType>));
        return (components_.find(type_ind) != components_.end());
    }

private:

    struct CObjectImplInterface {
        virtual size_t hash() const = 0;
        virtual size_t id() const noexcept = 0;
    };

    template <typename T>
    struct CObjectImpl : CObjectImplInterface
    {
    public:
        explicit CObjectImpl(const T &o)
        :
        value_ { o }
        , id_ { std::hash<size_t>()(std::chrono::steady_clock::now().time_since_epoch().count()) }
        {}

        explicit CObjectImpl(T &&o)
        :
        value_ { std::move(o) }
        , id_ { std::hash<size_t>()(std::chrono::steady_clock::now().time_since_epoch().count()) }
        {}

        [[nodiscard]] size_t hash() const override {
            return std::hash<size_t>()(id_);
        }

        [[nodiscard]] size_t id() const noexcept override {
            return id_;
        }

        const T &get() const noexcept {
            return value_;
        }

    private:
        T value_;
        const size_t id_;
    };

    std::shared_ptr<CObjectImplInterface> impl_;
    std::unordered_set<std::string> user_tags_; // reserved for a later use
    std::unordered_set<std::string> system_tags_;
    std::unordered_map<std::type_index, std::shared_ptr<CoreObject> > components_;
};
}

#endif