#ifndef RCBE_RGBACOLOR_HPP
#define RCBE_RGBACOLOR_HPP

#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/fundamentals/types.hpp>

#include <rdmn-engine/public_api.hpp>

namespace rcbe::visual {
R_PUBLIC_API class RGBAColor {
public:

    static constexpr size_t DIMENSION = 4;

    using ValueType = rcbe::core::EngineScalar;
    using StorageType = rcbe::math::Vector<ValueType, DIMENSION>;

public:

    R_PUBLIC_API RGBAColor() = default;

    R_PUBLIC_API explicit RGBAColor(uint32_t hex_code);

    R_PUBLIC_API RGBAColor(
        const rcbe::core::EngineScalar r,
        const rcbe::core::EngineScalar g,
        const rcbe::core::EngineScalar b,
        const rcbe::core::EngineScalar a = 1.0);
    R_PUBLIC_API RGBAColor(
        const rcbe::core::EngineIntergral r,
        const rcbe::core::EngineIntergral g,
        const rcbe::core::EngineIntergral b,
        const rcbe::core::EngineIntergral a = 255);

    R_PUBLIC_API ~RGBAColor() = default;

    R_PUBLIC_API RGBAColor(const RGBAColor &other) = default;
    R_PUBLIC_API RGBAColor &operator=(const RGBAColor &other) = default;

    R_PUBLIC_API RGBAColor(RGBAColor &&other) = default;
    R_PUBLIC_API RGBAColor &operator=(RGBAColor &&other) = default;

    [[nodiscard]] R_PUBLIC_API const ValueType &r() const noexcept;
    R_PUBLIC_API ValueType &r();

    [[nodiscard]] R_PUBLIC_API const ValueType &g() const noexcept;
    R_PUBLIC_API ValueType &g();

    [[nodiscard]] R_PUBLIC_API const ValueType &b() const noexcept;
    R_PUBLIC_API ValueType &b();

    [[nodiscard]] R_PUBLIC_API const ValueType &a() const noexcept;
    R_PUBLIC_API ValueType &a();

    [[nodiscard]] R_PUBLIC_API std::array<typename StorageType::ValueType, DIMENSION> asArray() const;

    R_PUBLIC_API explicit operator uint32_t() const;

private:

    StorageType storage_;
};
}// namespace rcbe::visual

namespace nlohmann {
template <>
struct adl_serializer<rcbe::visual::RGBAColor> {
    static void to_json(nlohmann::json &j, const rcbe::visual::RGBAColor &c);
    static void from_json(const nlohmann::json &j, rcbe::visual::RGBAColor &c);
};
}// namespace nlohmann


#endif