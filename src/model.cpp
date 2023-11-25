#include <model.hpp>

#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <mesh.hpp>
#include <render_buffer.hpp>
#include <textures.hpp>

#include <assimp_to_glm.hpp>

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
        glm::vec2 texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x,
                                                                  mesh->mTextureCoords[0][i].y)
                                                      : glm::vec2(0.0f);

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

    RenderBuffer renderBuffer{vertices, indices, MESH_VERTEX_DESCRIPTORS};

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
                                                       aiProcess_GenSmoothNormals |
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
    m_BoneInfoMap = {};
    m_BoneCount = 0;

    processNode(scene->mRootNode, scene);
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
        glm::vec2 texCoords = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x,
                                                                  mesh->mTextureCoords[0][i].y)
                                                      : glm::vec2(0.0f);

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

    extractBoneWeights(vertices, mesh, scene);

    RenderBuffer renderBuffer{vertices, indices, SKINNED_MESH_VERTEX_DESCRIPTORS};

    // std::ofstream file;
    // file.open("../" + name + ".txt");

    // file << "Mesh " << name << ":\n";
    // for (unsigned int i = 0; i < vertices.size(); i++) {
    //     SkinnedMeshVertex vertex = vertices[i];
    //     for (unsigned char j = 0; j < 4; j++)
    //         file << "\tVertex id: " << i << "; BoneId: " << vertex.m_BoneIds[j] << "; BoneWeights: " << vertex.m_BoneWeights[j] << std::endl;
    //     file << std::endl;
    // }

    // file.close();

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
void SkinnedModel::extractBoneWeights(std::vector<SkinnedMeshVertex>& vertices, const aiMesh* mesh, const aiScene* scene) {
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
        int boneId = -1;
        std::string boneName = mesh->mBones[i]->mName.C_Str();

        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end()) {
            m_BoneInfoMap[boneName] = BoneInfo(m_BoneCount,
                                               AssimpToGlm::aiMatrix4x4ToGlm(mesh->mBones[i]->mOffsetMatrix));
            boneId = m_BoneCount++;
        } else
            boneId = m_BoneInfoMap[boneName].getId();
        assert(boneId != -1);

        for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
            unsigned int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;

            assert(vertexId <= vertices.size());
            for (unsigned int k = 0; k < 4; k++) {
                if (vertices[vertexId].m_BoneIds[k] < 0) {
                    vertices[vertexId].m_BoneWeights[k] = weight;
                    vertices[vertexId].m_BoneIds[k] = boneId;
                    break;
                }
            }
        }
    }
}
#pragma endregion
