#ifndef RCBE_ENGINE_TEXTURE_HPP
#define RCBE_ENGINE_TEXTURE_HPP

#include <memory>

#include <rcbe-engine/datamodel/math/DynamicMatrix.hpp>
#include <rcbe-engine/fundamentals/types.hpp>
#include <rcbe-engine/datamodel/visual/texture_types.hpp>

namespace rcbe::visual {

class TextureImplementation;

class Texture {
public:

    using ImageBodyType = math::DynamicMatrix<uint32_t>;

    Texture() = delete;
    Texture(const core::EnginePath &path, texture_config config, bool for_rasterizer = false);
    ~Texture();

    Texture(const Texture &other);
    Texture &operator=(const Texture &other);

    Texture(Texture &&other);
    Texture &operator=(Texture &&other);

    [[nodiscard]] size_t getWidth() const;
    [[nodiscard]] size_t getHeight() const;
    [[nodiscard]] size_t getBitsPerPixel() const;

    void bind(const size_t index = 0) const;
    void unbind() const;

    [[nodiscard]] const ImageBodyType &getImageBody() const;
private:

    std::unique_ptr<TextureImplementation> impl_;
};
}

#endif //RCBE_ENGINE_TEXTURE_HPP
