#include <data_types/visual/RGBAColor.hpp>

namespace
{
static constexpr rcbe::core::EngineIntergral COLOR_COMPONENT_DIVIDER = 255;

bool checkRange(const rcbe::core::EngineScalar &value)
{
    return  (value >= 0.0) && (value <= 1.0);
}

void checkStorageInRange(const rcbe::visual::RGBAColor::storage_type &storage)
{
    for(size_t i = 0; i < storage.dimension; ++i)
    {
        if (!checkRange(storage[i]))
            throw std::runtime_error("Value of one of the components is not in range!");
    }
}
}

namespace rcbe::visual
{
RGBAColor::RGBAColor(
    const rcbe::core::EngineScalar r, 
    const rcbe::core::EngineScalar g, 
    const rcbe::core::EngineScalar b, 
    const rcbe::core::EngineScalar a
)
:
_storage(r, g, b, a)
{
    checkStorageInRange(_storage);
}

RGBAColor::RGBAColor(
    const rcbe::core::EngineIntergral r, 
    const rcbe::core::EngineIntergral g, 
    const rcbe::core::EngineIntergral b, 
    const rcbe::core::EngineIntergral a
)
:
_storage(
    r / COLOR_COMPONENT_DIVIDER,
    g / COLOR_COMPONENT_DIVIDER,
    b / COLOR_COMPONENT_DIVIDER,
    a / COLOR_COMPONENT_DIVIDER
)
{
    checkStorageInRange(_storage);
}

const RGBAColor::value_type &RGBAColor::r() const
{
    return _storage.x();
}

RGBAColor::value_type &RGBAColor::r()
{
    return _storage.x();
}

const RGBAColor::value_type &RGBAColor::g() const
{
    return _storage.y();
}

RGBAColor::value_type &RGBAColor::g()
{
    return _storage.y();
}

const RGBAColor::value_type &RGBAColor::b() const
{
    return _storage.z();
}

RGBAColor::value_type &RGBAColor::b()
{
    return _storage.z();
}

const RGBAColor::value_type &RGBAColor::a() const
{
    return _storage.w();
}

RGBAColor::value_type &RGBAColor::a()
{
    return _storage.w();
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