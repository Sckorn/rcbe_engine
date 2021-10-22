#include <unordered_set>
#include <unordered_map>

#include <boost/log/trivial.hpp>

#include <rcbe-engine/datamodel/rendering/Material.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <datamodel/rendering/Material.hpp>

static const std::unordered_set<std::string> allowed_texture_extensions = {
        ".tga",
        ".TGA"
};

namespace rcbe::rendering {
Material::Material(
        std::vector<ShaderArguments> &&shaders,
        std::vector<TextureArguments> &&textures,
        bool deferred)
:
shader_args_(std::move(shaders))
, texture_args_(std::move(textures))
, deferred_(deferred) {
    if (!deferred_) {
        initializeDeferredMaterial();
    }
}

Material::Material(std::vector<Shader> &&shaders, std::vector<visual::Texture> &&textures, bool deferred)
:
shaders_(std::move(shaders))
, textures_(std::move(textures))
, deferred_(deferred) {
    if (!deferred_) {
        initializeDeferredMaterial();
    }
}

Material::Material(const Material &other)
:
shader_args_(other.shader_args_)
, texture_args_(other.texture_args_)
, shaders_(other.shaders_)
, textures_(other.textures_)
, deferred_(other.deferred_) {
    if (!deferred_)
        initializeDeferredMaterial();
}

Material &Material::operator=(const Material &other) {
    if (this == &other)
        return *this;

    shader_args_ = other.shader_args_;
    texture_args_ = other.texture_args_;
    shaders_ = other.shaders_;
    textures_ = other.textures_;
    deferred_ = other.deferred_;

    if (!deferred_)
        initializeDeferredMaterial();

    return *this;
}

void Material::useShaderProgram() const {
    if (shader_program_)
        shader_program_->use();
    else
        throw std::runtime_error("Trying to use nullptr shader program!");
}

void Material::apply() const {
    if (!shader_program_) {
        initializeDeferredMaterial();
        if (!shader_program_)
            throw std::runtime_error("Shader program for material wasn't created!");
    }

    shader_program_->use();

    size_t counter = 0;
    for (const auto &t : textures_) {
        shader_program_->setInteger("texture" + std::to_string(counter), counter);
        t.bind(counter++);
    }
}

void Material::initializeDeferredMaterial() const {
    if (!shader_args_.empty()) {
        std::vector<Shader> shaders_tmp;
        shaders_tmp.reserve(shader_args_.size());
        for (const auto &s : shader_args_) {
            shaders_tmp.emplace_back(s.path, s.type);
        }

        if (shaders_tmp.empty())
            throw std::runtime_error("Material is constructed with empty set of shaders, and will have no effect! First");

        shader_program_ = std::make_unique<ShaderProgram>(std::move(shaders_tmp));

        for (const auto &t : texture_args_) {
            if (!std::filesystem::exists(t.path) || !std::filesystem::is_regular_file(t.path))
                throw std::runtime_error("Texture doesn't exist or not a file!");

            if (!allowed_texture_extensions.contains(t.path.extension().string()))
                throw std::runtime_error("Texture extension is not supported!");

            textures_.emplace_back(t.path, t.config, t.for_rasterizer);
        }

        if (textures_.empty())
            BOOST_LOG_TRIVIAL(warning) << "No textures supplied to material!";
    } else {
        if (shaders_.empty())
            throw std::runtime_error("Material is constructed with empty set of shaders, and will have no effect!");

        shader_program_ = std::make_unique<ShaderProgram>(std::move(shaders_));
    }

    deferred_ = false;
}

bool Material::isDeferred() const noexcept {
    return deferred_;
}

const std::unique_ptr<ShaderProgram> &Material::getShaderProgram() const noexcept {
    return shader_program_;
}

const std::vector<visual::Texture> &Material::getTextures() const noexcept {
    return textures_;
}
}