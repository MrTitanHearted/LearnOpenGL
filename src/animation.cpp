#include <animation.hpp>

void Animation::ReadMissingBones(const aiAnimation *animation, SkinnedModel &model)
{
    int size = animation->mNumChannels;

    auto &boneInfoMap = model.GetBoneInfoMap();
    int &boneCount = model.GetBoneCount();

    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
    }

    BoneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData &dest, const aiNode *src)
{
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

Animation::Animation(const std::string &animationPath, SkinnedModel *model)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    auto animation = scene->mAnimations[0];
    Duration = animation->mDuration;
    TicksPerSecond = animation->mTicksPerSecond;
    aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
    globalTransformation = globalTransformation.Inverse();
    ReadHierarchyData(RootNode, scene->mRootNode);
    ReadMissingBones(animation, *model);
}

Bone *Animation::FindBone(const std::string &name)
{
    auto iter = std::find_if(Bones.begin(), Bones.end(), [&](const Bone &Bone)
                             { return Bone.GetBoneName() == name; });
    if (iter == Bones.end())
        return nullptr;
    else
        return &(*iter);
}
