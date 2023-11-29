#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>

class Bone {
   public:
    Bone() = default;

    Bone(int id, glm::mat4 offsetMatrix)
        : m_Id(id), m_OffsetMatrix(offsetMatrix) {}

    unsigned int getId() const { return m_Id; }
    const glm::mat4 &getOffsetMatrix() { return m_OffsetMatrix; }

   protected:
    unsigned int m_Id;
    glm::mat4 m_OffsetMatrix;

    friend class Animation;
};

struct KeyPosition {
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation {
    glm::quat rotation;
    float timeStamp;
};

struct KeyScaling {
    glm::vec3 scaling;
    float timeStamp;
};

class KeyFrames {
   public:
    KeyFrames(const char *name, int boneId, const aiNodeAnim *channel);
    KeyFrames(const std::string &name, int boneId, const aiNodeAnim *channel)
        : KeyFrames(name.c_str(), boneId, channel) {}

    void update(float animationTime);

    glm::mat4 getLocalTransform() const { return m_LocalTransform; }
    std::string getBoneName() const { return m_BoneName; }
    unsigned int getBoneId() const { return m_BoneId; }

    unsigned int getPositionIndex(float animationTime) const;
    unsigned int getRotationIndex(float animationTime) const;
    unsigned int getScalingIndex(float animationTime) const;

   private:
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScaling> m_Scalings;

    glm::mat4 m_LocalTransform;
    std::string m_BoneName;
    unsigned int m_BoneId;

    static float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
        return (animationTime - lastTimeStamp) / (nextTimeStamp - lastTimeStamp);
    }

    glm::mat4 interpolatePosition(float animationTime) const;
    glm::mat4 interpolateRotation(float animationTime) const;
    glm::mat4 interpolateScaling(float animationTime) const;
};