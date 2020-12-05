#ifndef RCBE_ENGINE_DIMENSIONS_HPP
#define RCBE_ENGINE_DIMENSIONS_HPP

#include <rcbe-engine/fundamentals/types.hpp>

#include <nlohmann/json_fwd.hpp>

namespace rcbe::core {

template <typename Value = core::EngineIntergral>
struct dimensions_impl {
    using ValueType = Value;

    ValueType width;
    ValueType height;
};

using Dimensions = dimensions_impl<core::EngineIntergral>;
using ScalarDimensions = dimensions_impl<core::EngineScalar>;

}

namespace nlohmann {
template <>
struct adl_serializer<rcbe::core::Dimensions> {
    static void to_json(json& j, const rcbe::core::Dimensions& dim);
    static void from_json(const json& j, rcbe::core::Dimensions& dim);
};
}

#endif //RCBE_ENGINE_DIMENSIONS_HPP
