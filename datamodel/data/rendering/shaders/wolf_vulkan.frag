#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_debug_printf : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec2 fragTexCoord;
layout (location = 2) flat in int textureSamplerIdx;

layout (location = 0) out vec4 outColor;

layout (binding = 1) uniform sampler texSampler;
layout (binding = 2) uniform texture2D textures[4];

void main()
{
    outColor = vec4(0.0, 1.0, 0.0, 1.0);
}