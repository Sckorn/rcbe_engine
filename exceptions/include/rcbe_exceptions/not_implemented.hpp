#ifndef RCBE_ENGINE_NOT_IMPLEMENTED_HPP
#define RCBE_ENGINE_NOT_IMPLEMENTED_HPP

#include <stdexcept>

namespace rcbe::exception {
class NotImplementedException : public std::exception {
public:

    NotImplementedException() noexcept = default;
    ~NotImplementedException() noexcept override = default;
    explicit NotImplementedException(const char *what) noexcept
        : what_ {what} {}

    explicit NotImplementedException(std::string what) noexcept
        : what_ {std::move(what)} {}

    [[nodiscard]] const char *
    what() const noexcept override {
        return what_.c_str();
    }

private:

    std::string what_;
};
}// namespace rcbe::exception

#endif//RCBE_ENGINE_NOT_IMPLEMENTED_HPP
