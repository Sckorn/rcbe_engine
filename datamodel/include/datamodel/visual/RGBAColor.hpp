#ifndef RCBE_RGBACOLOR_HPP
#define RCBE_RGBACOLOR_HPP

#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>

namespace rcbe::visual
{
class RGBAColor
{
public:
    static constexpr size_t DIMENSION = 4;

    using ValueType = rcbe::core::EngineScalar;
    using StorageType = rcbe::math::Vector<ValueType, DIMENSION>;
public:
    RGBAColor() = default;

    RGBAColor(uint32_t hex_code);

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

    const ValueType &r() const noexcept;
    ValueType &r();

    const ValueType &g() const noexcept;
    ValueType &g();

    const ValueType &b() const noexcept;
    ValueType &b();

    const ValueType &a() const noexcept;
    ValueType &a();

    std::array<typename StorageType::ValueType, DIMENSION> asArray() const;

    explicit operator uint32_t() const;

private:
    StorageType storage_;
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