#ifndef RCBE_ENGINE_GLRASTERIZERTEXTURE_HPP
#define RCBE_ENGINE_GLRASTERIZERTEXTURE_HPP

#include <rcbe-engine/datamodel/rendering/rasterizer_texture_types.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>

namespace rdmn::render {
class RasterizerTextureImplementation {
public:
    RasterizerTextureImplementation() = delete;
    RasterizerTextureImplementation(rasterizer_texture_config config, rcbe::visual::TexturePtr texture);
    ~RasterizerTextureImplementation() = default;

    void bind(const size_t index = 0) const;
    void unbind() const;

    [[nodiscard]] bool deferred() const noexcept;

private:
    const bool deferred_ = true;
    size_t id_ = 0;
    rasterizer_texture_config config_;
};
}

#endif //RCBE_ENGINE_GLRASTERIZERTEXTURE_HPP
