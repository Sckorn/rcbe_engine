#ifndef RCBE_ENGINE_ABSTRACTDELEGATE_HPP
#define RCBE_ENGINE_ABSTRACTDELEGATE_HPP

#include <memory>

#include <boost/log/trivial.hpp>

#include <common/delegate/Delegate.hpp>

namespace rcbe::core {
class AbstractDelegate {
public:
    template <typename ... Signature>
    AbstractDelegate(Delegate<Signature...>&& d)
            :
            impl_ { std::make_shared< DelegateImpl < Delegate < Signature... > >  >(std::move(d)) }
    {
        static_assert(std::is_rvalue_reference_v<decltype(d)>, "Abstract delegate owns the actual delegate invoked, provide rvalue reference!!!");
    }

    template <typename ... Signature>
    void invoke(Signature&& ... s) const {
        auto actual_type_ptr = std::dynamic_pointer_cast<DelegateImpl<Delegate<void, Signature...>>>(impl_);
        if (actual_type_ptr) {
            actual_type_ptr->invoke(std::forward<Signature>(s)...);
        } else {
            throw std::runtime_error("Can't cast delegate to actual type");
        }
    }

    [[nodiscard]] size_t size() const noexcept {
        return impl_->size();
    }

    [[nodiscard]] size_t max_size() const noexcept {
        return impl_->max_size();
    }

    template <typename ... Signature>
    Delegate<void, Signature...>& as() {
        auto actual_type_ptr = std::dynamic_pointer_cast<DelegateImpl<Delegate<void, Signature...>>>(impl_);
        if (actual_type_ptr) {
            return actual_type_ptr->get();
        } else {
            throw std::runtime_error("Can't cast delegate to actual type");
        }
    }

    template <typename ... Signature>
    const Delegate<void, Signature...>& as() const {
        auto actual_type_ptr = std::dynamic_pointer_cast<DelegateImpl<Delegate<void, Signature...>>>(impl_);
        if (actual_type_ptr) {
            return actual_type_ptr->get();
        } else {
            throw std::runtime_error("Can't cast delegate to actual type");
        }
    }

    template <typename ... Signature>
    void add(typename Delegate<void, Signature&&...>::invocation_type&& inv) {
        auto actual_type_ptr = std::dynamic_pointer_cast<DelegateImpl<Delegate<void, Signature...>>>(impl_);
        if (actual_type_ptr) {
            actual_type_ptr->add(std::move(inv));
        } else {
            throw std::runtime_error("Can't cast delegate to actual type");
        }
    }

private:

    // TODO maybe try to use CRPT, for this
    struct DelegateImplInterface {
        [[nodiscard]] virtual size_t max_size() const noexcept = 0;
        [[nodiscard]] virtual size_t size() const noexcept = 0;
    };

    template <typename T>
    struct DelegateImpl : public DelegateImplInterface {
    public:
        DelegateImpl(T&& delegate)
                :
                delegate_ { std::move(delegate) }
        {}

        [[nodiscard]] size_t max_size() const noexcept override {
            return delegate_.max_size();
        }

        [[nodiscard]] size_t size() const noexcept override {
            return delegate_.size();
        }

        template <typename ... Signature>
        void invoke(Signature&& ... s) const {
            delegate_.invoke(std::forward<Signature>(s)...);
        }

        void add(typename T::invocation_type&& inv) {
            delegate_.add(std::move(inv));
        }

        T& get() {
            return delegate_;
        }

        const T& get() const {
            return delegate_;
        }

    private:
        T delegate_;
    };

    std::shared_ptr<DelegateImplInterface> impl_;
};
}

#endif //RCBE_ENGINE_ABSTRACTDELEGATE_HPP
