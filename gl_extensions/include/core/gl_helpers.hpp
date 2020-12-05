#ifndef RCBE_ENGINE_GL_HELPERS_HPP
#define RCBE_ENGINE_GL_HELPERS_HPP

#include <cstdint>
#include <array>

#include <GL/gl.h>

namespace rcbe::core {
static constexpr std::size_t gl_transform_size = 16; //TODO: constant to Upper case

template <typename MatrixType, typename UnderlyingValueType = GLfloat>
MatrixType gl_matrix_get(GLenum matrix_type) {
    UnderlyingValueType m[gl_transform_size];
    glGetFloatv(matrix_type, m);

    return MatrixType(m);
}
}

#endif //RCBE_ENGINE_GL_HELPERS_HPP
