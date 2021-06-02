#ifndef RCBE_ENGINE_SHADER_HPP
#define RCBE_ENGINE_SHADER_HPP

#include <filesystem>

#include <rcbe-engine/fundamentals/types.hpp>

#include <GL/gl.h>

namespace rcbe::rendering {
static constexpr size_t SHADER_ERROR_MSG_SIZE = 512;

enum class ShaderType {
    fragment = GL_FRAGMENT_SHADER,
    vertex = GL_VERTEX_SHADER,
    geometry = GL_GEOMETRY_SHADER,
    tesselation_evaluation = GL_TESS_EVALUATION_SHADER,
    tesselation_control = GL_TESS_CONTROL_SHADER,
    compute = GL_COMPUTE_SHADER,
    unknown = -1,
};

class Shader {
public:
    using ShaderHandle = size_t;

    Shader() = delete;
    ~Shader();

    Shader(const Shader &other);
    Shader &operator=(const Shader &other);

    Shader(Shader &&other);
    Shader &operator=(Shader &&other);
    Shader(const core::EnginePath &path, ShaderType type);

    [[nodiscard]] const ShaderHandle &handle() const noexcept;
    [[nodiscard]] const ShaderHandle &compile();
    [[nodiscard]] const core::EnginePath &path() const noexcept;
    [[nodiscard]] bool isCompiled() const noexcept;
    [[nodiscard]] ShaderType type() const noexcept;

    [[nodiscard]] int maxVertexAttribs() const noexcept;

private:
    void deleteShader() const;

    static int MAXIMUM_VERTEX_ATTRIBUTES;

    ShaderType type_ = ShaderType::unknown;
    core::EnginePath file_path_;
    bool compiled_ = false;
    ShaderHandle shader_handle_;

    std::string source_code_;
};

}

#endif // RCBE_ENGINE_SHADER_HPP
