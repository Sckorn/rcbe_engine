#ifndef RDMN_ENGINE_GLSHADER_HPP
#define RDMN_ENGINE_GLSHADER_HPP

#include <rcbe-engine/datamodel/rendering/Shader.hpp>

namespace rdmn::render {
class GLShader {
public:
    using ShaderHandle = size_t;

    GLShader() = delete;
    ~GLShader();

    GLShader(const GLShader &other);
    GLShader &operator=(const GLShader &other);

    GLShader(GLShader &&other);
    GLShader &operator=(GLShader &&other);

    GLShader(const rcbe::core::EnginePath &path, ShaderType type, [[maybe_unused]] ShaderState state = ShaderState::source);

    [[nodiscard]] const ShaderHandle &handle() const noexcept;
    [[nodiscard]] const ShaderHandle &compile();
    [[nodiscard]] const rcbe::core::EnginePath &path() const noexcept;
    [[nodiscard]] bool isCompiled() const noexcept;
    [[nodiscard]] ShaderType type() const noexcept;

    [[nodiscard]] int maxVertexAttribs() const noexcept;

private:
    void deleteShader() const;

    static int MAXIMUM_VERTEX_ATTRIBUTES;

    ShaderType type_ = ShaderType::unknown;
    const ShaderState state_ = ShaderState::source;
    rcbe::core::EnginePath file_path_;
    bool compiled_ = false;
    ShaderHandle shader_handle_;

    std::string source_code_;
};
}

#endif //RDMN_ENGINE_GLSHADER_HPP
