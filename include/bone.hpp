#pragma once

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <assimp_glm_helper.hpp>

struct BoneInfo
{
    int id;
    glm::mat4x4 offset;
};

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScaling
{
    glm::vec3 scale;
    float timeStamp;
};

class Bone
{
private:
    std::vector<KeyPosition> Positions;
    std::vector<KeyRotation> Rotations;
    std::vector<KeyScaling> Scalings;
    int NumPositions;
    int NumRotations;
    int NumScalings;

    glm::mat4x4 LocalTransform;
    std::string Name;
    int ID;
public:
    Bone(const std::string &name, int ID, const aiNodeAnim *channel);
    void Update(float animationTime);
    glm::mat4x4 GetLocalTransform() { return LocalTransform; }
    std::string GetBoneName() const { return Name; }
    int GetBoneID() { return ID; }

    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScalingIndex(float animationTime);
private:
    float GetScalingFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
    glm::mat4x4 InterpolatePosition(float animationTime);
    glm::mat4x4 InterpolateRotation(float animationTime);
    glm::mat4x4 InterpolateScaling(float animationTime);
};