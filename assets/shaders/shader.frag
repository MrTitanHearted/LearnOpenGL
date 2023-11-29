#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureDiffuse0;

void main() {
    FragColor = texture(textureDiffuse0, TexCoords);
}