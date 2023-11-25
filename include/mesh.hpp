#pragma once

#include <map>

#include <glm/glm.hpp>

#include <render_buffer.hpp>
#include <shader.hpp>
#include <textures.hpp>

class MeshVertex {
   public:
    MeshVertex(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec2 texCoords = glm::vec2(0.0f, 0.0f))
        : m_Position(position),
          m_Normal(normal),
          m_TexCoords(texCoords) {}

    static inline std::vector<VertexDescriptor> s_VertexDescriptors = {
        VertexDescriptor{0, 3, 0x1406, 8, 0},  // GL_FLOAT = 0x1406
        VertexDescriptor{1, 3, 0x1406, 8, 3},  // GL_FLOAT = 0x1406
        VertexDescriptor{2, 2, 0x1406, 8, 6},  // GL_FLOAT = 0x1406
    };

   private:
    glm::vec3 m_Position;
    glm::vec3 m_Normal;
    glm::vec2 m_TexCoords;
};

class SkinnedMeshVertex {
   public:
    SkinnedMeshVertex(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec2 texCoords = glm::vec2(0.0f, 0.0f),
                      glm::ivec4 boneIDs = glm::ivec4(-1, -1, -1, -1),
                      glm::vec4 boneWeights = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f))
        : m_Position(position),
          m_Normal(normal),
          m_TexCoords(texCoords),
          m_BoneIDs(boneIDs),
          m_BoneWeights(boneWeights) {}

    static inline std::vector<VertexDescriptor> s_VertexDescriptors = {
        VertexDescriptor{0, 3, 0x1406, 16, 0},   // GL_FLOAT = 0x1406
        VertexDescriptor{1, 3, 0x1406, 16, 3},   // GL_FLOAT = 0x1406
        VertexDescriptor{2, 2, 0x1406, 16, 6},   // GL_FLOAT = 0x1406
        VertexDescriptor{3, 4, 0x1402, 16, 8},   // GL_INT = 0x1404
        VertexDescriptor{4, 4, 0x1406, 16, 12},  // GL_FLOAT = 0x1406
    };

   private:
    glm::vec3 m_Position;
    glm::vec3 m_Normal;
    glm::vec2 m_TexCoords;
    glm::ivec4 m_BoneIDs;
    glm::vec4 m_BoneWeights;
};

class Mesh {
   public:
    Mesh() {}

    Mesh(RenderBuffer renderBuffer,
         std::map<std::string, Texture2D> textures = std::map<std::string, Texture2D>(),
         std::string name = "Mesh");

    void render(const Shader& shader) const;

    const std::map<std::string, Texture2D>& getTextures() const;

    std::string getName() const;

   private:
    RenderBuffer m_RenderBuffer;
    std::map<std::string, Texture2D> m_Textures;
    std::string m_Name;
};