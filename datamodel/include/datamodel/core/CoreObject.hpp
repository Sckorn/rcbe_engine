#ifndef RCBE_COREOBJECT_HPP
#define RCBE_COREOBJECT_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace rcbe::core
{
class CoreObject
{
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

    template <typename T>
    void addComponent(std::string &&tag, T &&component) {
        auto [it, result] = system_tags_.insert(std::move(tag));
        CoreObject comp_co { std::forward<T>(component) };
        components_.insert_or_assign(*it, std::make_shared<CoreObject>(std::move(comp_co)));
    }

    std::shared_ptr<CoreObject> getComponent(const std::string &tag) const {
        auto it = system_tags_.find(tag);
        if (it != system_tags_.end()) {
            auto it_map = components_.find(*it);
            if (it_map != components_.end()) {
                return it_map->second;
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    [[nodiscard]] bool hasSystemTag(const std::string &tag) const {
        return (system_tags_.find(tag) != system_tags_.end());
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
    std::unordered_map<std::string, std::shared_ptr<CoreObject> > components_;
};
}

#endif