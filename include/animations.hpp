#pragma once

#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <bone.hpp>
#include <model.hpp>

struct AnimationChannel {
    glm::mat4 transformation;
    std::string name;
    std::vector<AnimationChannel> children;
};

class Animation {
   public:
    Animation() = default;

    Animation(const char *path, SkinnedModel *model);
    Animation(const std::string &path, SkinnedModel *model)
        : Animation(path.c_str(), model) {}

    KeyFrames *findKeyFrames(const std::string &name) {
        std::vector<KeyFrames>::iterator iter = std::find_if(m_KeyFrames.begin(), m_KeyFrames.end(),
                                                             [&](const KeyFrames &keyFrames) {
                                                                 return keyFrames.getBoneName() == name;
                                                             });
        if (iter == m_KeyFrames.end())
            return nullptr;
        else
            return &(*iter);
    }

    float getTicksPerSecond() const { return m_TicksPerSecond; }
    float getDuration() const { return m_Duration; }
    const AnimationChannel &getRootChannel() const { return m_RootChannel; }
    const std::map<std::string, Bone> &getBoneMap() const { return m_BoneMap; }

   private:
    void readMissingBones(const aiAnimation *animation, SkinnedModel &model);
    void readHierarchyData(AnimationChannel &dest, const aiNode *src);

    float m_Duration;
    unsigned int m_TicksPerSecond;

    AnimationChannel m_RootChannel;

    std::vector<KeyFrames> m_KeyFrames;
    std::map<std::string, Bone> m_BoneMap;
};