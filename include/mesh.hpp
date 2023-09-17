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

// struct SkinnedMeshVertex
// {
//     glm::vec3 Position;
//     glm::vec3 Normal;
//     glm::vec2 TexCoords;
//     int BoneIDs[MAX_BONE_INFLUENCE];
//     float Weights[MAX_BONE_INFLUENCE];
// };

// class Skinned
// {
// private:
//     std::vector<MeshVertex>   vertices;
//     std::vector<unsigned int> indices;
//     std::vector<MeshTexture>  textures;
//     std::string name;

//     unsigned int vao;
//     unsigned int vbo;
//     unsigned int ebo;

//     void setupSkinned();
// public:
//     Skinned(const char* name, std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    
//     void Render(Shader &shader);
// };

// Simple Cube
/*
    // std::vector<MeshVertex> vertices = {
    //     {glm::vec3(-1.0, -1.0, -1.0), glm::vec3(0.0), glm::vec2(0.0)},
    //     {glm::vec3(-1.0, -1.0,  1.0), glm::vec3(0.0), glm::vec2(0.0)},
    //     {glm::vec3(-1.0,  1.0, -1.0), glm::vec3(0.0), glm::vec2(0.0)},
    //     {glm::vec3(-1.0,  1.0,  1.0), glm::vec3(0.0), glm::vec2(0.0)},
    //     {glm::vec3( 1.0, -1.0, -1.0), glm::vec3(0.0), glm::vec2(0.0)},
    //     {glm::vec3( 1.0, -1.0,  1.0), glm::vec3(0.0), glm::vec2(0.0)},
    //     {glm::vec3( 1.0,  1.0, -1.0), glm::vec3(0.0), glm::vec2(0.0)},
    //     {glm::vec3( 1.0,  1.0,  1.0), glm::vec3(0.0), glm::vec2(0.0)},
    // };

    // std::vector<unsigned int> indices = {
    //     0, 1, 3,    0, 3, 2, // Left
    //     4, 5, 7,    4, 7, 6, // Right
    //     0, 1, 5,    0, 5, 4, // Bottom
    //     2, 3, 7,    2, 7, 6, // Top
    //     0, 4, 6,    0, 6, 2, // Back
    //     1, 5, 7,    1, 7, 3, // Front
    // };
*/
   