#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D textureDiffuse0;
uniform sampler2D textureSpecular0;

void main() {
    FragColor = texture(textureDiffuse0, TexCoords);
}