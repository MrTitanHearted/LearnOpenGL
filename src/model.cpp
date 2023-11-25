#include <model.hpp>

#include <map>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <mesh.hpp>
#include <render_buffer.hpp>
#include <textures.hpp>

#pragma region class Model
Model::Model(const char* path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
                                                       aiProcess_GenNormals |
                                                       aiProcess_GenUVCoords |
                                                       aiProcess_FlipUVs |
                                                       aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        std::cerr << "[ERROR]::FAILED_TO_LOAD_MODEL::path: "
                  << path << "\nError: "
                  << importer.GetErrorString() << std::endl;

    m_Path = path;
    m_Name = m_Path.substr(m_Path.find_last_of('/') + 1, m_Path.length());
    m_Directory = m_Path.substr(0, m_Path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::render(const Shader& shader) const {
    for (const Mesh& mesh : m_Meshes)
        mesh.render(shader);
}

std::string Model::getPath() const {
    return m_Path;
}

std::string Model::getName() const {
    return m_Name;
}

std::string Model::getDirectory() const {
    return m_Directory;
}

void Model::processNode(const aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        m_Meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene) {
    std::vector<MeshVertex> vertices{};
    std::vector<unsigned int> indices{};
    std::map<std::string, Texture2D> textures{};
    std::string name = mesh->mName.C_Str();

    vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position{mesh->mVertices[i].x,
                           mesh->mVertices[i].y,
                           mesh->mVertices[i].z};
        glm::vec3 normal{mesh->mNormals[i].x,
                         mesh->mNormals[i].y,
                         mesh->mNormals[i].z};
        glm::vec2 texCoords{};
        if (mesh->mTextureCoords[0]) {
            texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                  mesh->mTextureCoords[0][i].y);
        } else
            texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(MeshVertex{position, normal, texCoords});
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);

    if (mesh->mMaterialIndex >= 0) {
        loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, textures);
        loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, textures);
        loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_NORMALS, textures);
    }

    RenderBuffer renderBuffer{vertices, indices, MeshVertex::s_VertexDescriptors};

    return Mesh(renderBuffer, textures, name);
}

void Model::loadMaterialTextures(const aiMaterial* material, aiTextureType type, std::map<std::string, Texture2D>& textures) {
    std::string textureName{};
    switch (type) {
        case aiTextureType_DIFFUSE:
            textureName = "textureDiffuse";
            break;
        case aiTextureType_SPECULAR:
            textureName = "textureSpecular";
            break;
        case aiTextureType_NORMALS:
            textureName = "textureNormal";
            break;
        default:
            std::cerr << "[ERROR]::MODEL::LOAD_MATERIAL_TEXTURES::Can't identify the type of the texture!\n";
    }
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString name;
        material->GetTexture(type, i, &name);
        std::string path = m_Directory + "/" + name.C_Str();
        textureName += std::to_string(i);
        textures[textureName] = Texture2D(path);
    }
}
#pragma endregion

#pragma region class SkinnedModel
SkinnedModel::SkinnedModel(const char* path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
                                                       aiProcess_GenNormals |
                                                       aiProcess_GenUVCoords |
                                                       aiProcess_FlipUVs |
                                                       aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        std::cerr << "[ERROR]::FAILED_TO_LOAD_MODEL::path: "
                  << path << "\nError: "
                  << importer.GetErrorString() << std::endl;

    m_Path = path;
    m_Name = m_Path.substr(m_Path.find_last_of('/') + 1, m_Path.length());
    m_Directory = m_Path.substr(0, m_Path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void SkinnedModel::render(const Shader& shader) const {
    for (const Mesh& mesh : m_Meshes)
        mesh.render(shader);
}

std::string SkinnedModel::getPath() const {
    return m_Path;
}

std::string SkinnedModel::getName() const {
    return m_Name;
}

std::string SkinnedModel::getDirectory() const {
    return m_Directory;
}

void SkinnedModel::processNode(const aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        m_Meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh SkinnedModel::processMesh(const aiMesh* mesh, const aiScene* scene) {
    std::vector<SkinnedMeshVertex> vertices{};
    std::vector<unsigned int> indices{};
    std::map<std::string, Texture2D> textures{};
    std::string name = mesh->mName.C_Str();

    vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position{mesh->mVertices[i].x,
                           mesh->mVertices[i].y,
                           mesh->mVertices[i].z};
        glm::vec3 normal{mesh->mNormals[i].x,
                         mesh->mNormals[i].y,
                         mesh->mNormals[i].z};
        glm::vec2 texCoords{};
        if (mesh->mTextureCoords[0]) {
            texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                  mesh->mTextureCoords[0][i].y);
        } else
            texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(SkinnedMeshVertex{position, normal, texCoords});
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);

    if (mesh->mMaterialIndex >= 0) {
        loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, textures);
        loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, textures);
        loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_NORMALS, textures);
    }

    RenderBuffer renderBuffer{vertices, indices, MeshVertex::s_VertexDescriptors};

    return Mesh(renderBuffer, textures, name);
}

void SkinnedModel::loadMaterialTextures(const aiMaterial* material, aiTextureType type, std::map<std::string, Texture2D>& textures) {
    std::string textureName{};
    switch (type) {
        case aiTextureType_DIFFUSE:
            textureName = "textureDiffuse";
            break;
        case aiTextureType_SPECULAR:
            textureName = "textureSpecular";
            break;
        case aiTextureType_NORMALS:
            textureName = "textureNormal";
            break;
        default:
            std::cerr << "[ERROR]::MODEL::LOAD_MATERIAL_TEXTURES::Can't identify the type of the texture!\n";
    }
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString name;
        material->GetTexture(type, i, &name);
        std::string path = m_Directory + "/" + name.C_Str();
        textureName += std::to_string(i);
        textures[textureName] = Texture2D(path);
    }
}
#pragma endregion
