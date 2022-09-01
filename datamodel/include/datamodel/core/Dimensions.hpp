#ifndef RCBE_ENGINE_DIMENSIONS_HPP
#define RCBE_ENGINE_DIMENSIONS_HPP

#include <type_traits>

#include <rcbe-engine/fundamentals/types.hpp>

#include <nlohmann/json_fwd.hpp>

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
        return dimensions_impl<NonQualT>{static_cast<NonQualT>(width), static_cast<NonQualT>(height)};
    }
};

using Dimensions = dimensions_impl<EngineIntergral>;
using ScalarDimensions = dimensions_impl<EngineScalar>;

}

namespace nlohmann {
template <>
struct adl_serializer<rcbe::core::Dimensions> {
    static void to_json(json& j, const rcbe::core::Dimensions& dim);
    static void from_json(const json& j, rcbe::core::Dimensions& dim);
};
}

#endif //RCBE_ENGINE_DIMENSIONS_HPP
