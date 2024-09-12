#include <rdmn-engine/logger/trivial_logger.hpp>

#include <rcbe-engine/core/gl_extensions.hpp>
#include <rcbe-engine/datamodel/rendering/Shader.hpp>
#include <rcbe-engine/datamodel/rendering/ShaderProgram.hpp>

namespace rcbe::rendering {
ShaderProgram::ShaderProgram(ShaderCollection &&shaders)
    : shader_prog_handle_ {glCreateProgram()} {
    std::sort(shaders.begin(), shaders.end(), [](const auto &l, const auto &r) {
        return static_cast<int>(l.type()) > static_cast<int>(r.type());
    });

    for (auto &shader : shaders) {
        if (!shader.isCompiled())
            shader.compile();

        if (shader.isCompiled()) {
#ifdef RDMN_OPENGL
            glAttachShader(shader_prog_handle_, shader.handle());
#endif
        } else {
            RDMN_LOG(RDMN_LOG_WARN) << "Shader " << shader.handle() << " is not compiled! Skipping it!";
        }
    }

    glLinkProgram(shader_prog_handle_);

    int success;
    std::string info;
    info.reserve(rdmn::render::SHADER_ERROR_MSG_SIZE);
    info.resize(rdmn::render::SHADER_ERROR_MSG_SIZE);
    glGetProgramiv(shader_prog_handle_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_prog_handle_, rdmn::render::SHADER_ERROR_MSG_SIZE, nullptr, info.data());
        glDeleteProgram(shader_prog_handle_);
        throw std::runtime_error("Shader program linking error: " + info);
    }
}

ShaderProgram::~ShaderProgram() {
    if (shader_prog_handle_ != 0) {
        glDeleteProgram(shader_prog_handle_);
    }
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) {
    this->shader_prog_handle_ = other.shader_prog_handle_;

    other.shader_prog_handle_ = 0;
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) {
    if (this == &other)
        return *this;

    this->shader_prog_handle_ = other.shader_prog_handle_;

    other.shader_prog_handle_ = 0;

    return *this;
}

void ShaderProgram::use() const {
    glUseProgram(shader_prog_handle_);
}


void ShaderProgram::setBool(const std::string &name, bool value) const {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void ShaderProgram::setInteger(const std::string &name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setVector(const std::string &name, const math::Vector4f &value) const {
    glUniform4fv(getUniformLocation(name), 1, value.rawData());
}

void ShaderProgram::setVector(const std::string &name, const math::Vector3f &value) const {
    glUniform3fv(getUniformLocation(name), 1, value.rawData());
}

void ShaderProgram::setVector(const std::string &name, const math::Vector2f &value) const {
    glUniform2fv(getUniformLocation(name), 1, value.rawData());
}

void ShaderProgram::setVector(const std::string &name, const math::Vector4d &value) const {
    auto v = value.convertUnderlyingType<float>();
    glUniform4fv(getUniformLocation(name), 1, v.rawData());
}

void ShaderProgram::setVector(const std::string &name, const math::Vector3d &value) const {
    auto v = value.convertUnderlyingType<float>();
    glUniform3fv(getUniformLocation(name), 1, v.rawData());
}

void ShaderProgram::setVector(const std::string &name, const math::Vector2d &value) const {
    auto v = value.convertUnderlyingType<float>();
    glUniform2fv(getUniformLocation(name), 1, v.rawData());
}

void ShaderProgram::setMatrix(const std::string &name, const math::MatrixColumnMajorAdaptor<core::EngineScalar> &value) const {
    auto m = value.convertValuesTo<float>();
    glUniformMatrix4fv(getUniformLocation(name), 1, false, m.getRaw());
}

void ShaderProgram::setMatrix(const std::string &name, const math::MatrixColumnMajorAdaptor<float> &value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, false, value.getRaw());
}

void ShaderProgram::setMatrix(const std::string &name, const math::Matrix4x4f &value) const {
    math::MatrixColumnMajorAdaptor<float> adaptor(value);
    glUniformMatrix4fv(getUniformLocation(name), 1, false, adaptor.getRaw());
}

void ShaderProgram::setMatrix(const std::string &name, const math::Matrix4x4 &value) const {
    math::MatrixColumnMajorAdaptor<float> adaptor(value);
    glUniformMatrix4fv(getUniformLocation(name), 1, false, adaptor.getRaw());
}

void ShaderProgram::setMatrix(const std::string &name, const math::Matrix3x3 &value) const {
    math::MatrixColumnMajorAdaptor<float> adaptor(value);
    glUniformMatrix3fv(getUniformLocation(name), 1, false, adaptor.getRaw());
}

#ifdef RCBE_DEBUG_MODE
ShaderProgram::ShaderProgramHandle ShaderProgram::getHandle() const {
    return shader_prog_handle_;
}
#endif

int ShaderProgram::getUniformLocation(const std::string &name) const {
    return glGetUniformLocation(shader_prog_handle_, name.c_str());
}
}// namespace rcbe::rendering