#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <textures.hpp>
#include <shaders.hpp>

#ifndef MAX_BONE_INFLUENCE
#define MAX_BONE_INFLUENCE 4
#endif

struct MeshVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct MeshTexture
{
    Texture2D texture;
    std::string type;
    std::string path;
};

class BaseMesh
{
private:
    std::vector<MeshVertex>   vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture>  textures;
    std::string name;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    void setupBaseMesh();
public:
    BaseMesh(const char* name, std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    
    void Render(Shader &shader);
};

struct SkinnedMeshVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    int BoneIDs[MAX_BONE_INFLUENCE];
    float Weights[MAX_BONE_INFLUENCE];
};

class SkinnedMesh
{
private:
    std::vector<SkinnedMeshVertex>   vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture>  textures;
    std::string name;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    void setupSkinnedMesh();
public:
    SkinnedMesh(const char* name, std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    
    void Render(Shader &shader);
};
