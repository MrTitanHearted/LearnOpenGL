#include <textures.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D(const char *path)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, NULL);
    int TextureFormat = GL_RGBA;
    if (nrChannels == 3)
    {
        TextureFormat = GL_RGB;
    }
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, TextureFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Texture2D::bind(const Shader *shader, const std::string &name, int value) const
{
    shader->use();
    shader->setInt(name, value);
    glActiveTexture(GL_TEXTURE0 + (unsigned int)value);
    glBindTexture(GL_TEXTURE_2D, id);
}

int Texture2D::get_width() const
{
    return width;
}

int Texture2D::get_height() const
{
    return height;
}

int Texture2D::get_numChannels() const
{
    return nrChannels;
}
