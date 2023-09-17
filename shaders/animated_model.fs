#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse0;

void main()
{
    FragColor = texture(texture_diffuse0, vec2(TexCoords.x, 1.0 - TexCoords.y));
}