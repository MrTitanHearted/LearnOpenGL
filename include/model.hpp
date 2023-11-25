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

    void render(const Shader &shader) const {
        for (const Mesh &mesh : m_Meshes)
            mesh.render(shader);
    }

    std::string getPath() const { return m_Path; }
    std::string getName() const { return m_Name; }
    std::string getDirectory() const { return m_Directory; }

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

    void render(const Shader &shader) const {
        for (const Mesh &mesh : m_Meshes)
            mesh.render(shader);
    }

    std::string getPath() const { return m_Path; }
    std::string getName() const { return m_Name; }
    std::string getDirectory() const { return m_Directory; }

    unsigned int getBoneCount() const { return m_BoneCount; }

   private:
    std::vector<Mesh> m_Meshes;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
    std::string m_Path;
    std::string m_Name;
    std::string m_Directory;

    unsigned int m_BoneCount;

    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(const aiMaterial *material, aiTextureType type, std::map<std::string, Texture2D> &textures);
    void extractBoneWeights(std::vector<SkinnedMeshVertex> &vertices, const aiMesh *mesh, const aiScene *scene);
};