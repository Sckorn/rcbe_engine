#include <rcbe-engine/datamodel/visual/RGBAColor.hpp>

namespace
{
static constexpr rcbe::core::EngineIntergral COLOR_COMPONENT_DIVIDER = 255;

bool checkRange(const rcbe::core::EngineScalar &value)
{
    return  (value >= 0.0) && (value <= 1.0);
}

void checkStorageInRange(const rcbe::visual::RGBAColor::StorageType &storage)
{
    for(size_t i = 0; i < storage.DIMENSION; ++i)
    {
        if (!checkRange(storage[i]))
            throw std::runtime_error("Value of one of the components is not in range!");
    }
}

uint8_t extract_color_byte(const uint32_t hex, uint8_t position) {
    const uint8_t check = 0b1111'1111;
    return (hex >> position * 8) & check;
}
}

namespace rcbe::visual
{
RGBAColor::RGBAColor(uint32_t hex_code)
:
storage_(
        extract_color_byte(hex_code, 3) / COLOR_COMPONENT_DIVIDER,
        extract_color_byte(hex_code, 2) / COLOR_COMPONENT_DIVIDER,
        extract_color_byte(hex_code, 1) / COLOR_COMPONENT_DIVIDER,
        extract_color_byte(hex_code, 0) / COLOR_COMPONENT_DIVIDER
)
{
    checkStorageInRange(storage_);
}

RGBAColor::RGBAColor(
    const rcbe::core::EngineScalar r, 
    const rcbe::core::EngineScalar g, 
    const rcbe::core::EngineScalar b, 
    const rcbe::core::EngineScalar a
)
:
storage_(r, g, b, a)
{
    checkStorageInRange(storage_);
}

RGBAColor::RGBAColor(
    const rcbe::core::EngineIntergral r, 
    const rcbe::core::EngineIntergral g, 
    const rcbe::core::EngineIntergral b, 
    const rcbe::core::EngineIntergral a
)
:
storage_(
    r / COLOR_COMPONENT_DIVIDER,
    g / COLOR_COMPONENT_DIVIDER,
    b / COLOR_COMPONENT_DIVIDER,
    a / COLOR_COMPONENT_DIVIDER
)
{
    checkStorageInRange(storage_);
}

const RGBAColor::ValueType &RGBAColor::r() const noexcept
{
    return storage_.x();
}

RGBAColor::ValueType &RGBAColor::r()
{
    return storage_.x();
}

const RGBAColor::ValueType &RGBAColor::g() const noexcept
{
    return storage_.y();
}

RGBAColor::ValueType &RGBAColor::g()
{
    return storage_.y();
}

const RGBAColor::ValueType &RGBAColor::b() const noexcept
{
    return storage_.z();
}

RGBAColor::ValueType &RGBAColor::b()
{
    return storage_.z();
}

const RGBAColor::ValueType &RGBAColor::a() const noexcept
{
    return storage_.w();
}

RGBAColor::ValueType &RGBAColor::a()
{
    return storage_.w();
}
}

namespace nlohmann
{
void adl_serializer<rcbe::visual::RGBAColor>::to_json(nlohmann::json &j, const rcbe::visual::RGBAColor &c)
{
    j = {
        {"r", c.r()},
        {"g", c.g()},
        {"b", c.b()},
        {"a", c.a()},
    };
}

void adl_serializer<rcbe::visual::RGBAColor>::from_json(const nlohmann::json &j, rcbe::visual::RGBAColor &c)
{
    c = rcbe::visual::RGBAColor {
        j.at("r").get<rcbe::core::EngineScalar>(),
        j.at("g").get<rcbe::core::EngineScalar>(),
        j.at("b").get<rcbe::core::EngineScalar>(),
        j.at("a").get<rcbe::core::EngineScalar>()
    };
}
} // namespace nlohmann