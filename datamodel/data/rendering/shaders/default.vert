#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 ourTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
    gl_Position = perspective * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor * vec4(aNorm, 1.0);
    ourTexCoord = aTexCoord;
}