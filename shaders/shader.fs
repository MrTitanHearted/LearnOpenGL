#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTexture;
uniform sampler2D otherTexture;

void main()
{
    FragColor = mix(texture(myTexture, TexCoord), texture(otherTexture, TexCoord), 0.2);
}