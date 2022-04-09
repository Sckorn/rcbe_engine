#ifndef RCBE_ENGINE_SHADERPROGRAM_HPP
#define RCBE_ENGINE_SHADERPROGRAM_HPP

#include <vector>
#include <memory>

#include <rcbe-engine/datamodel/math/Vector.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/math/MatrixColumnMajorAdaptor.hpp>

/// TODO: fix this nested namespace later
namespace rdmn::render {
class Shader;
}

namespace rcbe::rendering {

using ShaderCollection = std::vector<rdmn::render::Shader>;

class ShaderProgram {
public:
    using ShaderProgramHandle = size_t;

    ShaderProgram() = delete;
    explicit ShaderProgram(ShaderCollection &&shaders);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram &other) = delete;
    ShaderProgram &operator=(const ShaderProgram &other) = delete;

    ShaderProgram(ShaderProgram &&other);
    ShaderProgram &operator=(ShaderProgram &&other);

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInteger(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVector(const std::string &name, const math::Vector4d &value) const;
    void setVector(const std::string &name, const math::Vector3d &value) const;
    void setVector(const std::string &name, const math::Vector2d &value) const;
    void setVector(const std::string &name, const math::Vector4f &value) const;
    void setVector(const std::string &name, const math::Vector3f &value) const;
    void setVector(const std::string &name, const math::Vector2f &value) const;
    void setMatrix(const std::string &name, const math::MatrixColumnMajorAdaptor<core::EngineScalar> &value) const;
    void setMatrix(const std::string &name, const math::MatrixColumnMajorAdaptor<float> &value) const;
    void setMatrix(const std::string &name, const math::Matrix4x4f &value) const;
    void setMatrix(const std::string &name, const math::Matrix4x4 &value) const;
    void setMatrix(const std::string &name, const math::Matrix3x3 &value) const;


#ifdef RCBE_DEBUG_MODE
    [[nodiscard]] ShaderProgramHandle getHandle() const;
#endif

private:
    [[nodiscard]] int getUniformLocation(const std::string &name) const;

    ShaderProgramHandle shader_prog_handle_ = 0;
};

using ShaderProgramPtr = std::unique_ptr<ShaderProgram>;
using ShaderProgramConstPtr = std::unique_ptr<const ShaderProgram>;
}

#endif //RCBE_ENGINE_SHADERPROGRAM_HPP
