#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 ourTexCoord;
uniform float customGreen;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec4 FlickeringGreenColor = vec4(ourColor.r, customGreen, ourColor.b, ourColor.a);
    FragColor = mix(texture(texture0, ourTexCoord), texture(texture1, ourTexCoord), 0.5) * FlickeringGreenColor;
}