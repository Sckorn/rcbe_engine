#ifndef RCBE_ENGINE_MATERIAL_HPP
#define RCBE_ENGINE_MATERIAL_HPP

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rcbe-engine/fundamentals/types.hpp>

#ifdef RDMN_VULKAN
#include <vulkan/vulkan.hpp>
#endif

#ifdef RDMN_OPENGL
#include <rcbe-engine/datamodel/rendering/ShaderProgram.hpp>
#endif
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/datamodel/rendering/rasterizer_texture_types.hpp>
#include <rcbe-engine/datamodel/rendering/RasterizerTexture.hpp>
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
        MaterialConfig() = default;
        explicit MaterialConfig(std::vector<ShaderArguments> &&shaders);
        MaterialConfig(std::vector<ShaderArguments> &&shaders, std::vector<TextureArguments> &&textures);
        ~MaterialConfig() = default;

        [[nodiscard]] const std::vector<ShaderArguments> &getShaderArgs() const noexcept;
        [[nodiscard]] const std::vector<TextureArguments> &getTextureArgs() const noexcept;

    private:
        std::vector<ShaderArguments> shader_args_;
        std::vector<TextureArguments> texture_args_;
    };

    using MaterialTexMetaData = std::unordered_map<size_t, rcbe::visual::TexturePtr>; // object hash to vtex ptr

    using TextureType = rdmn::render::RasterizerTexture;
    using ShaderType = rdmn::render::Shader;

#ifdef RDMN_VULKAN
    using TextureStorage = std::unordered_map<rcbe::visual::TexturePtr, std::shared_ptr<TextureType>>;
#endif

#ifdef RDMN_OPENGL
    using TextureStorage = std::vector<TextureType>;
    using ShaderStorage = std::vector<ShaderType>;
#endif

    Material() = delete;
    explicit Material(MaterialConfig &&config, bool deferred = false);
    Material(ShaderType &&vertex, ShaderType &&fragment, bool deferred = false);
#ifdef RDMN_OPENGL
    Material(std::vector<ShaderType> &&shaders, std::vector<TextureType> &&textures);
#endif
    ~Material() = default;

    Material(Material &&other) = default;
    Material &operator=(Material &&other) = default;

    [[nodiscard]] bool isDeferred() const noexcept;

    [[nodiscard]] const TextureStorage &getTextures() const noexcept;
    [[nodiscard]] TextureStorage &getTextures() noexcept;

    void initializeDeferredMaterial() const;

#ifdef RDMN_VULKAN
  typename TextureStorage::iterator pushTexture(std::shared_ptr<TextureType> t);

  bool addObjectMetadata(const size_t object_hash, const rcbe::visual::TexturePtr &vtex);

  const std::unique_ptr<ShaderType> &getVertex() const;
  const std::unique_ptr<ShaderType> &getFragment() const;

  std::unique_ptr<ShaderType> &getVertex();
  std::unique_ptr<ShaderType> &getFragment();
#endif

#ifdef RDMN_OPENGL
  void apply() const;
  void useShaderProgram() const;
  [[nodiscard]] const std::unique_ptr<ShaderProgram> &getShaderProgram() const noexcept;
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
}



#endif //RCBE_ENGINE_MATERIAL_HPP


