#ifndef RCBE_ENGINE_MATERIAL_HPP
#define RCBE_ENGINE_MATERIAL_HPP

#include <vector>
#include <memory>

#include <rcbe-engine/fundamentals/types.hpp>

#include <rcbe-engine/datamodel/rendering/ShaderProgram.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>

namespace rcbe::rendering {
class Material {
public:

    struct ShaderArguments {
        core::EnginePath path;
        ShaderType type;
    };

    struct TextureArguments {
        core::EnginePath path;
        visual::texture_config config;
        bool for_rasterizer = false;
    };

    Material() = delete;
    Material(std::vector<ShaderArguments> &&shaders, std::vector<TextureArguments> &&textures, bool deferred = false);
    Material(std::vector<Shader> &&shaders, std::vector<visual::Texture> &&textures, bool deferred = false);
    ~Material() = default;

    Material(const Material &other);
    Material &operator=(const Material &other);

    Material(Material &&other) = default;
    Material &operator=(Material &&other) = default;

    void useShaderProgram() const;
    void apply() const;

    [[nodiscard]] bool isDeferred() const noexcept;

    [[nodiscard]] const std::unique_ptr<ShaderProgram> &getShaderProgram() const noexcept;
    [[nodiscard]] const std::vector<visual::Texture> &getTextures() const noexcept;

    void initializeDeferredMaterial() const;

private:
    std::vector<ShaderArguments> shader_args_;
    std::vector<TextureArguments> texture_args_;

    mutable std::unique_ptr<ShaderProgram> shader_program_ = nullptr;
    mutable std::vector<rendering::Shader> shaders_;
    mutable std::vector<visual::Texture> textures_;

    mutable bool deferred_ = false;
};
}

#endif //RCBE_ENGINE_MATERIAL_HPP
