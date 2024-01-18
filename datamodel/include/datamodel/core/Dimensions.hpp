#ifndef RCBE_ENGINE_DIMENSIONS_HPP
#define RCBE_ENGINE_DIMENSIONS_HPP

#include <type_traits>

#include <nlohmann/json_fwd.hpp>

#include <rcbe-engine/fundamentals/types.hpp>

namespace rcbe::core {

template <typename Value = EngineIntergral>
struct dimensions_impl {
    using ValueType = Value;

    ValueType width;
    ValueType height;

    /// TODO: consider making typelist helper to specify a list of types that can be used with this template
    /// I mean dimension_impl @sckorn
    template <typename T, typename = std::enable_if_t<
                              std::is_convertible_v<T, EngineScalar> || std::is_convertible_v<T, EngineIntergral>, void>>
    [[nodiscard]] dimensions_impl<std::remove_cvref_t<T>> convertUnerlying() const {
        using NonQualT = std::remove_cvref_t<T>;
        return dimensions_impl<NonQualT> {static_cast<NonQualT>(width), static_cast<NonQualT>(height)};
    }
};

using IntegralDimensions = dimensions_impl<core::EngineIntergral>;
using ScalarDimensions = dimensions_impl<core::EngineScalar>;
/// TODO: need to figure out what to do if we redefine core::EngineScalar to be float
using FloatDimensions = dimensions_impl<float>;

}// namespace rcbe::core

namespace nlohmann {
template <>
struct adl_serializer<rcbe::core::IntegralDimensions> {
    static void to_json(json &j, const rcbe::core::IntegralDimensions &dim);
    static void from_json(const json &j, rcbe::core::IntegralDimensions &dim);
};
}// namespace nlohmann

#endif//RCBE_ENGINE_DIMENSIONS_HPP
