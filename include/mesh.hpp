#pragma once

#include <map>

#include <glm/glm.hpp>

#include <render_buffer.hpp>
#include <shader.hpp>
#include <textures.hpp>

class MeshVertex {
   public:
    MeshVertex(glm::vec3 position = glm::vec3(0.0f),
               glm::vec3 normal = glm::vec3(0.0f),
               glm::vec2 texCoords = glm::vec2(0.0f))
        : m_Position(position),
          m_Normal(normal),
          m_TexCoords(texCoords) {}

    glm::vec3 m_Position;
    glm::vec3 m_Normal;
    glm::vec2 m_TexCoords;
};

static const std::vector<VertexDescriptor> MESH_VERTEX_DESCRIPTORS = {
    VertexDescriptor{0, 3, 0x1406, sizeof(MeshVertex), offsetof(MeshVertex, m_Position)},   // GL_FLOAT = 0x1406
    VertexDescriptor{1, 3, 0x1406, sizeof(MeshVertex), offsetof(MeshVertex, m_Normal)},     // GL_FLOAT = 0x1406
    VertexDescriptor{2, 2, 0x1406, sizeof(MeshVertex), offsetof(MeshVertex, m_TexCoords)},  // GL_FLOAT = 0x1406
};

class SkinnedMeshVertex {
   public:
    SkinnedMeshVertex(glm::vec3 position = glm::vec3(0.0f),
                      glm::vec3 normal = glm::vec3(0.0f),
                      glm::vec2 texCoords = glm::vec2(0.0f),
                      glm::ivec4 boneIds = glm::ivec4(-1),
                      glm::vec4 boneWeights = glm::vec4(0.0f))
        : m_Position(position),
          m_Normal(normal),
          m_TexCoords(texCoords),
          m_BoneIds(boneIds),
          m_BoneWeights(boneWeights) {}

    glm::vec3 m_Position;
    glm::vec3 m_Normal;
    glm::vec2 m_TexCoords;
    glm::ivec4 m_BoneIds;
    glm::vec4 m_BoneWeights;
};

static const std::vector<VertexDescriptor> SKINNED_MESH_VERTEX_DESCRIPTORS = {
    VertexDescriptor{0, 3, 0x1406, sizeof(SkinnedMeshVertex), offsetof(SkinnedMeshVertex, m_Position)},     // GL_FLOAT = 0x1406
    VertexDescriptor{1, 3, 0x1406, sizeof(SkinnedMeshVertex), offsetof(SkinnedMeshVertex, m_Normal)},       // GL_FLOAT = 0x1406
    VertexDescriptor{2, 2, 0x1406, sizeof(SkinnedMeshVertex), offsetof(SkinnedMeshVertex, m_TexCoords)},    // GL_FLOAT = 0x1406
    VertexDescriptor{3, 4, 0x1402, sizeof(SkinnedMeshVertex), offsetof(SkinnedMeshVertex, m_BoneIds)},      // GL_INT = 0x1404
    VertexDescriptor{4, 4, 0x1406, sizeof(SkinnedMeshVertex), offsetof(SkinnedMeshVertex, m_BoneWeights)},  // GL_FLOAT = 0x1406
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