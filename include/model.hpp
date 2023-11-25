#pragma once

#include <vector>
#include <map>

#include <assimp/scene.h>

#include <shader.hpp>
#include <mesh.hpp>
#include <textures.hpp>
#include <bone.hpp>

class Model {
   public:
    Model(const char *path);

    void render(const Shader &shader) const;

    std::string getPath() const;
    std::string getName() const;
    std::string getDirectory() const;

   private:
    std::vector<Mesh> m_Meshes;
    std::string m_Path;
    std::string m_Name;
    std::string m_Directory;

    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(const aiMaterial *material, aiTextureType type, std::map<std::string, Texture2D> &textures);
};

class SkinnedModel {
   public:
    SkinnedModel(const char *path);

    void render(const Shader &shader) const;

    std::string getPath() const;
    std::string getName() const;
    std::string getDirectory() const;

   private:
    std::vector<Mesh> m_Meshes;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    std::string m_Path;
    std::string m_Name;
    std::string m_Directory;

    unsigned int m_BoneCounter;

    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(const aiMaterial *material, aiTextureType type, std::map<std::string, Texture2D> &textures);
    void extractBoneWeights(std::vector<SkinnedMeshVertex> &vertices, const aiMesh *mesh, const aiScene *scene);
};