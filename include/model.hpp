#pragma once

#include <string>
#include <vector>
#include <map>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <stb_image.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <bone.hpp>
#include <mesh.hpp>
#include <shaders.hpp>

class BaseModel
{
private:
    std::vector<BaseMesh> meshes;
    std::vector<MeshTexture> loaded_textures;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    BaseMesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
public:
    BaseModel(const char *path)
    {
        loadModel(path);
    }

    void Render(Shader &shader);
};

class SkinnedModel
{
private:
    std::map<std::string, BoneInfo> bone_info_map;
    int bone_counter = 0;

    std::vector<SkinnedMesh> meshes;
    std::vector<MeshTexture> loaded_textures;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    void setVertexBoneDataToDefault(SkinnedMeshVertex &vertex);
    void setVertexBoneData(SkinnedMeshVertex &vertex, int boneID, float weight);
    void extractBoneWeightForVertices(std::vector<SkinnedMeshVertex> &vertices, aiMesh *mesh, const aiScene *scene);
    SkinnedMesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
public:
    auto& GetBoneInfoMap() { return bone_info_map; }
    int& GetBoneCount() { return bone_counter; }
    SkinnedModel(const char *path)
    {
        loadModel(path);
    }

    void Render(Shader &shader);
};
