#include "GLShader.hpp"

#include <fstream>

#include <rcbe-engine/core/gl_extensions.hpp>

#include <boost/log/trivial.hpp>

namespace rdmn::render {
int GLShader::MAXIMUM_VERTEX_ATTRIBUTES = 0;

GLShader::~GLShader() {
    glDeleteShader(shader_handle_);
}

GLShader::GLShader(const rcbe::core::EnginePath &path, ShaderType type, ShaderState state)
:
type_(type)
, state_(state)
, file_path_(path) {
    if (!std::filesystem::exists(file_path_)) {
        throw std::runtime_error(file_path_.string() + " doesn't exist!");
    }

    if (type_ == ShaderType::unknown) {
        throw std::runtime_error("Unknown shader type supplied!");
    }

    if (!MAXIMUM_VERTEX_ATTRIBUTES)
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MAXIMUM_VERTEX_ATTRIBUTES);


    shader_handle_ = glCreateShader(static_cast<GLenum>(type_));
}

GLShader::GLShader(const GLShader &other)
:
type_ { other.type_ }
, file_path_ { other.file_path_ } {
    this->shader_handle_ = glCreateShader(static_cast<GLenum>(this->type_));
}

GLShader &GLShader::operator=(const GLShader &other) {
    if (this == &other)
        return *this;

    this->file_path_ = other.file_path_;
    this->type_ = other.type_;
    this->shader_handle_ = glCreateShader(static_cast<GLenum>(this->type_));
    return *this;
}

GLShader::GLShader(GLShader &&other)
:
type_ { other.type_ }
, file_path_ { std::move(other.file_path_) } {
    this->shader_handle_ = glCreateShader(static_cast<GLenum>(this->type_));
    other.deleteShader();
}

GLShader &GLShader::operator=(GLShader &&other) {
    if (this == &other)
        return *this;
    this->file_path_ = std::move(other.file_path_);
    this->type_ = other.type_;
    this->shader_handle_ = glCreateShader(static_cast<GLenum>(this->type_));
    other.deleteShader();
    return *this;
}

const GLShader::ShaderHandle &GLShader::handle() const noexcept {
    return shader_handle_;
}

const GLShader::ShaderHandle &GLShader::compile() {
    if (!shader_handle_)
        throw std::runtime_error("Shader was created with error! " + file_path_.string());

    if (!compiled_) {
        source_code_.clear();

        std::ifstream ifs(file_path_, std::ios::in);

        source_code_ = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
        if (source_code_.empty())
            throw std::runtime_error("Couldn't read shader source code!");

        const auto * scode = source_code_.c_str();

        //**
        //
        // C Specification
        //  void glShaderSource(GLuint shader,
        //  	                GLsizei count,
        //  	                const GLchar **string,
        //  	                const GLint *length);
        //
        //Parameters
        //
        //  shader
        //
        //    Specifies the handle of the shader object whose source code is to be replaced.
        //  count
        //
        //    Specifies the number of elements in the string and length arrays.
        //  string
        //
        //    Specifies an array of pointers to strings containing the source code to be loaded into the shader.
        //  length
        //
        //    Specifies an array of string lengths.
        //
        //
        //
        // **//
        glShaderSource(shader_handle_, 1, &scode, nullptr);
        glCompileShader(shader_handle_);

        int success;
        std::string info;
        info.reserve(SHADER_ERROR_MSG_SIZE);
        info.resize(SHADER_ERROR_MSG_SIZE);
        glGetShaderiv(shader_handle_, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader_handle_, SHADER_ERROR_MSG_SIZE, nullptr, info.data());
            BOOST_LOG_TRIVIAL(warning) << "Shader compilation failed: " << info;
        }
        compiled_ = static_cast<bool>(success);
    }

    return shader_handle_;
}

const rcbe::core::EnginePath &GLShader::path() const noexcept {
    return file_path_;
}

bool GLShader::isCompiled() const noexcept {
    return compiled_;
}

ShaderType GLShader::type() const noexcept {
    return type_;
}

int GLShader::maxVertexAttribs() const noexcept {
    return MAXIMUM_VERTEX_ATTRIBUTES;
}

void GLShader::deleteShader() const {
    glDeleteShader(shader_handle_);
}
}