#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec2 fragTexCoord;
layout (location = 2) out int textureSamplerIdx;

layout (binding = 0) uniform uniform_buffer_object {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

/// TODO: add vec4 some_color to pass color from renderer and pass it to fragColor
layout( push_constant ) uniform constants {
    mat4 model_matrix;
    int sampler_index;
} PushConstants;


void main() {
    gl_Position = ubo.proj * ubo.view * PushConstants.model_matrix * vec4(aPos, 1.0);
    fragColor = aColor;
    fragTexCoord = aTexCoord;
    textureSamplerIdx = PushConstants.sampler_index;
}