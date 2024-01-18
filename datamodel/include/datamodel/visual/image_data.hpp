#ifndef RCBE_ENGINE_IMAGE_DATA_HPP
#define RCBE_ENGINE_IMAGE_DATA_HPP

#include <memory>

#include <rcbe-engine/datamodel/core/Dimensions.hpp>
#include <rcbe-engine/datamodel/math/DynamicMatrix.hpp>
#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/visual/texture_types.hpp>

namespace rdmn::vis {
struct image_data {
    using SinglePixelType = uint32_t;
    using PixelsType = rcbe::math::DynamicMatrix<SinglePixelType>;
    using PixelsStorage = std::shared_ptr<const PixelsType>;
    struct image_metadata {
        rcbe::math::Vector2i origin;
        rcbe::core::IntegralDimensions dimensions;
        uint8_t pixel_depth_bytes;
        rcbe::visual::TextureComponentOrder component_order;
    } metadata;

    PixelsStorage pixels;

    static PixelsStorage wrapPixelsInStorage(PixelsType &&p) {
        return std::make_shared<PixelsType>(std::move(p));
    }
};
}// namespace rdmn::vis

#endif//RCBE_ENGINE_IMAGE_DATA_HPP
