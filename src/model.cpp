#include <model.hpp>

#include <iostream>

#include <textures.hpp>
#include <assimp_glm_helper.hpp>

#define ASSIMP_IMPORT_FLAGS aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_JoinIdenticalVertices

void BaseModel::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, ASSIMP_IMPORT_FLAGS);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "[ERROR]: ASSIMP:\n" << importer.GetErrorString() << "\n";
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void BaseModel::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

BaseMesh BaseModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
    {
        MeshVertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vector;
            vector.x = mesh->mTextureCoords[0][i].x;
            vector.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0, 0.0);
        
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    
        std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return BaseMesh(mesh->mName.C_Str(), vertices, indices, textures);
}

std::vector<MeshTexture> BaseModel::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString filename;
        material->GetTexture(type, i, &filename);
        bool skip = false;
        for (unsigned int j = 0; j < loaded_textures.size(); j++)
        {
            if (std::strcmp(loaded_textures[j].path.data(), filename.C_Str()) == 0)
            {
                textures.push_back(loaded_textures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            std::string path = directory + '/' + std::string(filename.C_Str());

            MeshTexture texture;
            texture.texture = Texture2D(path.c_str());
            texture.type = typeName;
            texture.path = path;
            textures.push_back(texture);
            loaded_textures.push_back(texture);
        }
    }

    return textures;
}

void BaseModel::Render(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Render(shader);
}

void SkinnedModel::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, ASSIMP_IMPORT_FLAGS);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "[ERROR]: ASSIMP:\n" << importer.GetErrorString() << "\n";
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void SkinnedModel::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

void SkinnedModel::setVertexBoneDataToDefault(SkinnedMeshVertex &vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.BoneIDs[i] = -1;
        vertex.Weights[i] = 0.0f;
    }
}

void SkinnedModel::setVertexBoneData(SkinnedMeshVertex &vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (vertex.BoneIDs[i] < 0)
        {
            vertex.Weights[i] = weight;
            vertex.BoneIDs[i] = boneID;
            break;
        }
    }
}

void SkinnedModel::extractBoneWeightForVertices(std::vector<SkinnedMeshVertex> &vertices, aiMesh *mesh, const aiScene *scene)
{
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (bone_info_map.find(boneName) == bone_info_map.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = bone_counter;
            newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
            bone_info_map[boneName] = newBoneInfo;
            boneID = bone_counter;
            bone_counter++;
        }
        else
        {
            boneID = bone_info_map[boneName].id;
        }

        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; weightIndex++)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            
            assert(vertexId <= vertices.size());

            setVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

SkinnedMesh SkinnedModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    
    std::vector<SkinnedMeshVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
    {
        SkinnedMeshVertex vertex;

        setVertexBoneDataToDefault(vertex);

        vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
        vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vector;
            vector.x = mesh->mTextureCoords[0][i].x;
            vector.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0, 0.0);
        
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    
        std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    extractBoneWeightForVertices(vertices, mesh, scene);

    return SkinnedMesh(mesh->mName.C_Str(), vertices, indices, textures);
}

std::vector<MeshTexture> SkinnedModel::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString filename;
        material->GetTexture(type, i, &filename);
        bool skip = false;
        for (unsigned int j = 0; j < loaded_textures.size(); j++)
        {
            if (std::strcmp(loaded_textures[j].path.data(), filename.C_Str()) == 0)
            {
                textures.push_back(loaded_textures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            std::string path = directory + '/' + std::string(filename.C_Str());

            MeshTexture texture;
            texture.texture = Texture2D(path.c_str());
            texture.type = typeName;
            texture.path = path;
            textures.push_back(texture);
            loaded_textures.push_back(texture);
        }
    }

    return textures;
}

void SkinnedModel::Render(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Render(shader);
}
