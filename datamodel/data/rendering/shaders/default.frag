#version 330 core
out vec4 FragColor;
in vec4 ourColor;
uniform float customGreen;

void main()
{
    FragColor = vec4(ourColor.r, customGreen, ourColor.b, ourColor.a);
}