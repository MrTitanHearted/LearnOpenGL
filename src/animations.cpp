#include <animations.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <assimp_to_glm.hpp>

Animation::Animation(const aiAnimation* pAssimpAnimation, const aiScene* pScene, SkinnedModel* pModel) {
    m_Name = pAssimpAnimation->mName.C_Str();
    m_Duration = pAssimpAnimation->mDuration;
    m_TicksPerSecond = pAssimpAnimation->mTicksPerSecond;

    readHierarchyData(m_RootChannel, pScene->mRootNode);
    readMissingBones(pAssimpAnimation, *pModel);
}

void Animation::readMissingBones(const aiAnimation* animation, SkinnedModel& model) {
    unsigned int size = animation->mNumChannels;

    std::map<std::string, Bone>& boneMap = model.getMutBoneMap();
    unsigned int& boneCount = model.getMutBoneCount();

    for (unsigned int i = 0; i < size; i++) {
        const aiNodeAnim* channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.C_Str();

        if (boneMap.find(boneName) == boneMap.end())
            boneMap[boneName].m_Id = boneCount++;

        m_KeyFrames.push_back(KeyFrames(boneName, boneMap[boneName].m_Id, channel));
    }

    m_BoneMap = boneMap;
}

void Animation::readHierarchyData(AnimationChannel& dest, const aiNode* src) {
    assert(src);

    dest.name = src->mName.C_Str();
    dest.transformation = AssimpToGlm::aiMatrix4x4ToGlm(src->mTransformation);
    dest.children.reserve(src->mNumChildren);

    for (unsigned int i = 0; i < src->mNumChildren; i++) {
        AnimationChannel newChannel;
        readHierarchyData(newChannel, src->mChildren[i]);
        dest.children.push_back(newChannel);
    }
}

Animations::Animations(const char* path, SkinnedModel* pModel) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
                                                       aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        std::cerr << "[ERROR]::FAILED_TO_LOAD_MODEL::path: "
                  << path << "\nError: "
                  << importer.GetErrorString() << std::endl;

    for (unsigned int i = 0; i < scene->mNumAnimations; i++)
        m_Animations[scene->mAnimations[i]->mName.C_Str()] = Animation(scene->mAnimations[i], scene, pModel);
}
