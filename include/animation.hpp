#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <bone.hpp>
#include <model.hpp>

struct AssimpNodeData
{
    glm::mat4x4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
private:
    float Duration;
    int TicksPerSecond;
    std::vector<Bone> Bones;
    AssimpNodeData RootNode;
    std::map<std::string, BoneInfo> BoneInfoMap;

    void ReadMissingBones(const aiAnimation *animation, SkinnedModel &model);
    void ReadHierarchyData(AssimpNodeData &dest, const aiNode *src);

public:
    Animation() = default;

    Animation(const std::string &animationPath, SkinnedModel *model);
    ~Animation() {}

    Bone *FindBone(const std::string &name);

    inline float GetTicksPerSecond() { return TicksPerSecond; }
    inline float GetDuration() { return Duration; }
    inline const AssimpNodeData &GetRootNode() { return RootNode; }
    inline const std::map<std::string, BoneInfo> &GetBoneIDMap() { return BoneInfoMap; }
};