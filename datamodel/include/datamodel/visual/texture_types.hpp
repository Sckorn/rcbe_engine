#ifndef RCBE_ENGINE_TEXTURE_TYPES_HPP
#define RCBE_ENGINE_TEXTURE_TYPES_HPP

#include <rcbe-engine/datamodel/math/DynamicMatrix.hpp>

namespace rcbe::visual {

enum class TextureComponentOrder {
    RGB,
    RGBA,
    GBR,
    GBRA,
    ARGB
};

struct texture_config {
    using ImageBodyType = rcbe::math::DynamicMatrix<uint32_t>;
    TextureComponentOrder component_order = TextureComponentOrder::RGBA;
};

}// namespace rcbe::visual

#endif//RCBE_ENGINE_TEXTURE_TYPES_HPP
