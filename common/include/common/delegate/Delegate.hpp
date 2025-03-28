#ifndef RCBE_ENGINE_DELEGATE_HPP
#define RCBE_ENGINE_DELEGATE_HPP

#include <functional>
#include <type_traits>
#include <vector>

namespace rcbe::core {
static constexpr size_t DEFAULT_DELEGATE_SIZE = 100;


template <typename Ret, typename... Signature>
class R_PUBLIC_API Delegate;

//TODO: actually handle different return types
// consider introducing InvocationResult class
// initial declaration to implement
// template <typename Ret, typename ... Signature>
// class Delegate;
template <typename... Signature>
class R_PUBLIC_API Delegate<void, Signature...> {
public:

    using InvocationType = std::function<void(Signature &&...)>;

    explicit Delegate(size_t max_delegate_size = DEFAULT_DELEGATE_SIZE)
        : max_delegate_size_ {max_delegate_size} {
    }

    [[nodiscard]] size_t maxSize() const noexcept {
        return max_delegate_size_;
    }

    Delegate &add(InvocationType &&f) {
        if (invocation_list_.size() == max_delegate_size_) {
            throw std::runtime_error("Invocation list is full");
        }
        invocation_list_.push_back(std::move(f));
        return *this;
    }

    Delegate &operator+=(InvocationType &&f) {
        return add(std::move(f));
    }

    [[nodiscard]] size_t size() const noexcept {
        return invocation_list_.size();
    }

    void invoke(Signature &&...s) const {
        for (const auto &f : invocation_list_) {
            f(std::forward<Signature>(s)...);
        }
    }

    void operator()(Signature &&...s) const {
        invoke(std::forward<Signature>(s)...);
    }

private:

    const size_t max_delegate_size_;
    std::vector<InvocationType> invocation_list_;
};
}// namespace rcbe::core

#endif//RCBE_ENGINE_DELEGATE_HPP
