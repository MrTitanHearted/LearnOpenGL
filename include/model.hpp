#pragma once

#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <stb_image.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

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