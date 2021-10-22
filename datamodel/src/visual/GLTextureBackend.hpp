#ifndef RCBE_ENGINE_GLTEXTUREBACKEND_HPP
#define RCBE_ENGINE_GLTEXTUREBACKEND_HPP

#include <rcbe-engine/datamodel/visual/texture_types.hpp>
#include <rcbe-engine/datamodel/math/DynamicMatrix.hpp>

#include <GL/gl.h>

namespace rcbe::visual {
class TextureRasterizerBackendImpl {
public:
    TextureRasterizerBackendImpl() = delete;
    TextureRasterizerBackendImpl(
            const uint8_t * texture_raw_data,
            const texture_config &config,
            const size_t width,
            const size_t height);

    ~TextureRasterizerBackendImpl() = default;

    void bind(const size_t index = 0) const;
    void unbind() const;

private:
    size_t id_ = 0;
    const texture_config &config_;
};

}

#endif //RCBE_ENGINE_GLTEXTUREBACKEND_HPP
