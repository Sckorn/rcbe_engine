#include <unordered_map>
#include <unordered_set>

#include <boost/log/trivial.hpp>

#include <rcbe-engine/datamodel/rendering/Material.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/fundamentals/convinience.hpp>

static const std::unordered_set<std::string> allowed_texture_extensions = {
    ".tga",
    ".TGA"};

namespace rcbe::rendering {
Material::MaterialConfig::MaterialConfig(std::vector<ShaderArguments> &&shaders)
    : shader_args_(std::move(shaders)) {}

Material::MaterialConfig::MaterialConfig(
    std::vector<ShaderArguments> &&shaders,
    std::vector<TextureArguments> &&textures)
    : shader_args_(std::move(shaders))
    , texture_args_(std::move(textures)) {}

const std::vector<Material::ShaderArguments> &Material::MaterialConfig::getShaderArgs() const noexcept {
    return shader_args_;
}

const std::vector<Material::TextureArguments> &Material::MaterialConfig::getTextureArgs() const noexcept {
    return texture_args_;
}

Material::Material(ShaderType &&vertex, ShaderType &&fragment, bool deferred)
    :
#ifdef RDMN_VULKAN
    vertex_(std::make_unique<ShaderType>(std::move(vertex)))
    , fragment_(std::make_unique<ShaderType>(std::move(fragment)))
    ,
#endif
    deferred_(deferred) {
    if (!deferred_) {
        initializeDeferredMaterial();
    }
}

Material::Material(MaterialConfig &&config, bool deferred)
    : config_(std::move(config))
    , deferred_(deferred) {
    if (!deferred_) {
        initializeDeferredMaterial();
    }
}

#ifdef RDMN_OPENGL
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
#endif

void Material::initializeDeferredMaterial() const {
    BOOST_LOG_TRIVIAL(debug) << R_READABLE_FUNC_NAME;

    const auto &shader_args = config_.getShaderArgs();

    if (!shader_args.empty()) {
#ifdef RDMN_OPENGL
        std::vector<rdmn::render::Shader> shaders_tmp;
        shaders_tmp.reserve(shader_args.size());
        for (const auto &s : shader_args) {
            shaders_tmp.emplace_back(s.path, s.type, s.state);
        }

        if (shaders_tmp.empty())
            throw std::runtime_error("Material is constructed with empty set of shaders, and will have no effect! First");
        shader_program_ = std::make_unique<ShaderProgram>(std::move(shaders_tmp));
#elif defined(RDMN_VULKAN)
        if (shader_args.size() > 2)
            BOOST_LOG_TRIVIAL(warning) << "Too many shaders supplied, material expects one vertex and one fragment shader!";

        using HandlerType = std::function<void(const ShaderArguments &)>;
        const auto handlers = std::unordered_map<rdmn::render::ShaderType, HandlerType> {
            {rdmn::render::ShaderType::unknown,
             [](const ShaderArguments &) {
                 BOOST_LOG_TRIVIAL(warning) << "Unknown shader type, skipping!";
             }},
            {rdmn::render::ShaderType::vertex,
             [this](const ShaderArguments &a) {
                 vertex_ = std::make_unique<ShaderType>(a.path, a.type, a.state);
             }},
            {rdmn::render::ShaderType::fragment,
             [this](const ShaderArguments &a) {
                 fragment_ = std::make_unique<ShaderType>(a.path, a.type, a.state);
             }}};

        for (auto &&s : shader_args) {
            handlers.at(s.type)(s);
        }

#endif

        const auto &texture_args = config_.getTextureArgs();

        for (auto &&t : texture_args) {
#ifdef RDMN_OPENGL
            textures_.emplace_back(t.config, t.texture);
#elif defined(RDMN_VULKAN)
            auto rt = std::make_shared<TextureType>(t.config, t.texture);
            textures_.insert_or_assign(rt->getVisualTexturePtr(), std::move(rt));
#endif
        }

#ifdef RDMN_OPENGL
    } else {
        if (shaders_.empty())
            throw std::runtime_error("Material is constructed with empty set of shaders, and will have no effect!");

        shader_program_ = std::make_unique<ShaderProgram>(std::move(shaders_));
    }
#else
    }
#endif
}

bool Material::isDeferred() const noexcept {
    return deferred_;
}

const Material::TextureStorage &Material::getTextures() const noexcept {
    return textures_;
}

Material::TextureStorage &Material::getTextures() noexcept {
    return textures_;
}

#ifdef RDMN_VULKAN
typename Material::TextureStorage::iterator Material::pushTexture(std::shared_ptr<TextureType> t) {
    auto [it, res] = textures_.insert_or_assign(t->getVisualTexturePtr(), std::move(t));
    if (!res)
        BOOST_LOG_TRIVIAL(warning) << "Texture is already present";
    return it;
}

const std::unique_ptr<Material::ShaderType> &Material::getVertex() const {
    return vertex_;
}

const std::unique_ptr<Material::ShaderType> &Material::getFragment() const {
    return fragment_;
}

std::unique_ptr<Material::ShaderType> &Material::getVertex() {
    return vertex_;
}

std::unique_ptr<Material::ShaderType> &Material::getFragment() {
    return fragment_;
}

bool Material::addObjectMetadata(const size_t object_hash, const rcbe::visual::TexturePtr &vtex) {
    auto it = textures_.find(vtex);
    if (it == textures_.end())
        return false;

    auto [iit, res] = tex_meta_cache_.insert({object_hash, vtex});

    return true;
}
#endif

#ifdef RDMN_OPENGL
const std::unique_ptr<ShaderProgram> &Material::getShaderProgram() const noexcept {
    return shader_program_;
}

void Material::useShaderProgram() const {
    if (shader_program_)
        shader_program_->use();
    else
        throw std::runtime_error("Trying to use nullptr shader program!");
}
#endif
}// namespace rcbe::rendering