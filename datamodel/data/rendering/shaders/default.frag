#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 ourTexCoord;
uniform float customGreen;
uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    vec4 FlickeringGreenColor = vec4(ourColor.r, customGreen, ourColor.b, ourColor.a);
    FragColor = mix(texture(ourTexture, ourTexCoord), texture(ourTexture2, ourTexCoord), 0.5) * FlickeringGreenColor;
}