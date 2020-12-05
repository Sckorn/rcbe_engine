#ifndef RCBE_RGBACOLOR_HPP
#define RCBE_RGBACOLOR_HPP

#include <data_types/config/data_model_config.hpp>
#include <data_types/math/Vector.hpp>

namespace rcbe::visual
{
class RGBAColor
{
public:
    static constexpr size_t dimension = 4;

    using value_type = rcbe::core::EngineScalar;
    using storage_type = rcbe::math::Vector<rcbe::core::EngineScalar, dimension>;
public:
    RGBAColor() = default;
    // TODO: add hex ctor
    RGBAColor(
        const rcbe::core::EngineScalar r, 
        const rcbe::core::EngineScalar g, 
        const rcbe::core::EngineScalar b, 
        const rcbe::core::EngineScalar a = 1.0
    );
    RGBAColor(
        const rcbe::core::EngineIntergral r, 
        const rcbe::core::EngineIntergral g, 
        const rcbe::core::EngineIntergral b, 
        const rcbe::core::EngineIntergral a = 255
    );

    ~RGBAColor() = default;

    RGBAColor(const RGBAColor &other) = default;
    RGBAColor &operator=(const RGBAColor &other) = default;

    RGBAColor(RGBAColor &&other) = default;
    RGBAColor &operator=(RGBAColor &&other) = default;

    const value_type &r() const;
    value_type &r();

    const value_type &g() const;
    value_type &g();

    const value_type &b() const;
    value_type &b();

    const value_type &a() const;
    value_type &a();

private:
    storage_type _storage;
};
}

namespace nlohmann
{
template <>
struct adl_serializer<rcbe::visual::RGBAColor>
{
static void to_json(nlohmann::json &j, const rcbe::visual::RGBAColor &c);
static void from_json(const nlohmann::json &j, rcbe::visual::RGBAColor &c);
};
} // namespace nlohmann


#endif