#include <mesh.hpp>

Mesh::Mesh(RenderBuffer renderBuffer, std::map<std::string, Texture2D> textures, std::string name) {
    m_RenderBuffer = renderBuffer;
    m_Textures = textures;
    m_Name = name;
}

void Mesh::render(const Shader &shader) const {
    shader.use();
    unsigned int i = 0;
    for (const std::pair<std::string, Texture2D> &pair : m_Textures) {
        pair.second.bind(shader, pair.first, i);
        i++;
    }
    m_RenderBuffer.render();
}

const std::map<std::string, Texture2D> &Mesh::getTextures() const {
    return m_Textures;
}

std::string Mesh::getName() const {
    return m_Name;
}
