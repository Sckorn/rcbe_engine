#ifndef RCBE_ENGINE_MATERIAL_HPP
#define RCBE_ENGINE_MATERIAL_HPP

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rdmn-engine/public_api.hpp>

#include <rcbe-engine/fundamentals/types.hpp>

#ifdef RDMN_VULKAN
#include <vulkan/vulkan.hpp>
#endif

#ifdef RDMN_OPENGL
#include <rcbe-engine/datamodel/rendering/ShaderProgram.hpp>
#endif
#include <rcbe-engine/datamodel/rendering/RasterizerTexture.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/datamodel/rendering/rasterizer_texture_types.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>

namespace rcbe::rendering {

class Material {
public:

    struct ShaderArguments {
        core::EnginePath path;
        rdmn::render::ShaderType type;
        rdmn::render::ShaderState state;
    };

    struct TextureArguments {
        rcbe::visual::TexturePtr texture;
        rdmn::render::rasterizer_texture_config config;
    };

    class MaterialConfig {
    public:

        R_PUBLIC_API MaterialConfig() = default;
        R_PUBLIC_API explicit MaterialConfig(std::vector<ShaderArguments> &&shaders);
        R_PUBLIC_API MaterialConfig(std::vector<ShaderArguments> &&shaders, std::vector<TextureArguments> &&textures);
        R_PUBLIC_API ~MaterialConfig() = default;

        [[nodiscard]] R_PUBLIC_API const std::vector<ShaderArguments> &getShaderArgs() const noexcept;
        [[nodiscard]] R_PUBLIC_API const std::vector<TextureArguments> &getTextureArgs() const noexcept;

    private:

        std::vector<ShaderArguments> shader_args_;
        std::vector<TextureArguments> texture_args_;
    };

    using MaterialTexMetaData = std::unordered_map<size_t, rcbe::visual::TexturePtr>;// object hash to vtex ptr

    using TextureType = rdmn::render::RasterizerTexture;
    using ShaderType = rdmn::render::Shader;

#ifdef RDMN_VULKAN
    using TextureStorage = std::unordered_map<rcbe::visual::TexturePtr, std::shared_ptr<TextureType>>;
#endif

#ifdef RDMN_OPENGL
    using TextureStorage = std::vector<TextureType>;
    using ShaderStorage = std::vector<ShaderType>;
#endif

    R_PUBLIC_API Material() = delete;
    R_PUBLIC_API explicit Material(MaterialConfig &&config, bool deferred = false);
    R_PUBLIC_API Material(ShaderType &&vertex, ShaderType &&fragment, bool deferred = false);
#ifdef RDMN_OPENGL
    R_PUBLIC_API Material(std::vector<ShaderType> &&shaders, std::vector<TextureType> &&textures);
#endif
    R_PUBLIC_API ~Material() = default;

    R_PUBLIC_API Material(Material &&other) = default;
    R_PUBLIC_API Material &operator=(Material &&other) = default;

    [[nodiscard]] R_PUBLIC_API bool isDeferred() const noexcept;

    [[nodiscard]] R_PUBLIC_API const TextureStorage &getTextures() const noexcept;
    [[nodiscard]] R_PUBLIC_API TextureStorage &getTextures() noexcept;

    R_PUBLIC_API void initializeDeferredMaterial() const;

#ifdef RDMN_VULKAN
    typename TextureStorage::iterator pushTexture(std::shared_ptr<TextureType> t);

    R_PUBLIC_API bool addObjectMetadata(const size_t object_hash, const rcbe::visual::TexturePtr &vtex);

    R_PUBLIC_API const std::unique_ptr<ShaderType> &getVertex() const;
    R_PUBLIC_API const std::unique_ptr<ShaderType> &getFragment() const;

    R_PUBLIC_API std::unique_ptr<ShaderType> &getVertex();
    R_PUBLIC_API std::unique_ptr<ShaderType> &getFragment();
#endif

#ifdef RDMN_OPENGL
    R_PUBLIC_API void apply() const;
    R_PUBLIC_API void useShaderProgram() const;
    [[nodiscard]] R_PUBLIC_API const std::unique_ptr<ShaderProgram> &getShaderProgram() const noexcept;
#endif

private:

    MaterialConfig config_;
    mutable TextureStorage textures_;

#ifdef RDMN_OPENGL
    mutable std::unique_ptr<ShaderProgram> shader_program_ = nullptr;
    mutable ShaderStorage shaders_;
#endif

#ifdef RDMN_VULKAN
    mutable std::unique_ptr<ShaderType> vertex_ = nullptr;
    mutable std::unique_ptr<ShaderType> fragment_ = nullptr;

    MaterialTexMetaData tex_meta_cache_;
#endif

    bool deferred_ = false;
};
}// namespace rcbe::rendering


#endif//RCBE_ENGINE_MATERIAL_HPP
