#include <textures.hpp>

#include <iostream>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D(const char* path, std::vector<Parameter> params) {
    glGenTextures(1, &m_Id);
    glBindTexture(GL_TEXTURE_2D, m_Id);

    for (const Parameter& param : params)
        glTexParameteri(GL_TEXTURE_2D, param.m_ParamName, param.m_Param);

    unsigned char* data = stbi_load(path, &m_Width, &m_Height, &m_ChannelsCount, 0);

    if (data) {
        int format = GL_RGBA;
        if (m_ChannelsCount == 3)
            format = GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "[ERROR]::TEXTURE_LOADING_FAILED!!! Path was: " << path << std::endl;
    }

    stbi_image_free(data);
}

void Texture2D::activate(const Shader& shader, const char* name, unsigned int index) const {
    shader.use();
    shader.setInt(name, index);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_Id);
}
