#ifndef RCBE_ENGINE_GL_HELPERS_HPP
#define RCBE_ENGINE_GL_HELPERS_HPP

#include <cstdint>
#include <array>

#include <GL/gl.h>

namespace rcbe::core {
static constexpr std::size_t GL_TRANSFORM_SIZE = 16;

template <typename MatrixType, typename UnderlyingValueType = GLfloat>
MatrixType gl_matrix_get(GLenum matrix_type) {
    UnderlyingValueType m[GL_TRANSFORM_SIZE];
    glGetFloatv(matrix_type, m);

    return MatrixType(m);
}
}

#endif //RCBE_ENGINE_GL_HELPERS_HPP
