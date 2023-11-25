#include <textures.hpp>

#include <iostream>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D(const char* path, std::vector<Parameter> params) {
    if (s_Cache.find(path) != s_Cache.end()) {
        m_Id = s_Cache[path].m_Id;
        m_ChannelsCount = s_Cache[path].m_ChannelsCount;
        m_Width = s_Cache[path].m_Width;
        m_Height = s_Cache[path].m_Height;
    } else {
        glGenTextures(1, &m_Id);
        glBindTexture(GL_TEXTURE_2D, m_Id);

        for (const Parameter& param : params)
            glTexParameteri(GL_TEXTURE_2D, param.m_ParamName, param.m_Param);

        unsigned char* data = stbi_load(path, &m_Width, &m_Height, &m_ChannelsCount, 0);

        if (data) {
            int format = GL_RGBA;
            if (m_ChannelsCount == 3)
                format = GL_RGB;
            if (m_ChannelsCount == 1)
                format = GL_RED;
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "[ERROR]::TEXTURE_LOADING_FAILED!!! Path was: " << path << std::endl;
        }

        stbi_image_free(data);
    }

    m_Path = path;
    m_Name = m_Path.substr(m_Path.find_last_of('/') + 1, m_Path.length());
    m_Directory = m_Path.substr(0, m_Path.find_last_of('/'));
}

void Texture2D::bind(const Shader& shader, const char* name, unsigned int index) const {
    shader.use();
    shader.set(name, index);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_Id);
}

std::string Texture2D::getPath() const {
    return m_Path;
}

std::string Texture2D::getName() const {
    return m_Name;
}

std::string Texture2D::getDirectory() const {
    return m_Directory;
}
