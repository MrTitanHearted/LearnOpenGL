#include <bone.hpp>

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <assimp_to_glm.hpp>

KeyFrames::KeyFrames(const char* name, int boneId, const aiNodeAnim* channel)
    : m_BoneName(name), m_BoneId(boneId), m_LocalTransform(1.0f) {
    m_Positions.reserve(channel->mNumPositionKeys);
    m_Rotations.reserve(channel->mNumRotationKeys);
    m_Scalings.reserve(channel->mNumRotationKeys);

    for (unsigned int i = 0; i < channel->mNumPositionKeys; i++) {
        aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
        float timeStamp = channel->mPositionKeys[i].mTime;
        KeyPosition data;
        data.position = AssimpToGlm::aiVector3ToGlm(aiPosition);
        data.timeStamp = timeStamp;
        m_Positions.push_back(data);
    }

    for (unsigned int i = 0; i < channel->mNumRotationKeys; i++) {
        aiQuaternion aiRotation = channel->mRotationKeys[i].mValue;
        float timeStamp = channel->mRotationKeys[i].mTime;
        KeyRotation data;
        data.rotation = AssimpToGlm::aiQuaternionToGlm(aiRotation);
        data.timeStamp = timeStamp;
        m_Rotations.push_back(data);
    }

    for (unsigned int i = 0; i < channel->mNumScalingKeys; i++) {
        aiVector3D aiScaling = channel->mScalingKeys[i].mValue;
        float timeStamp = channel->mScalingKeys[i].mTime;
        KeyScaling data;
        data.scaling = AssimpToGlm::aiVector3ToGlm(aiScaling);
        data.timeStamp = timeStamp;
        m_Scalings.push_back(data);
    }
}

void KeyFrames::update(float animationTime) {
    glm::mat4 translation = interpolatePosition(animationTime);
    glm::mat4 rotation = interpolateRotation(animationTime);
    glm::mat4 scaling = interpolateScaling(animationTime);
    m_LocalTransform = translation * rotation * scaling;
}

unsigned int KeyFrames::getPositionIndex(float animationTime) const {
    for (unsigned int i = 0; i < m_Positions.size() - 1; i++)
        if (animationTime < m_Positions[i + 1].timeStamp)
            return i;
    assert(0);
}

unsigned int KeyFrames::getRotationIndex(float animationTime) const {
    for (unsigned int i = 0; i < m_Rotations.size() - 1; i++)
        if (animationTime < m_Rotations[i + 1].timeStamp)
            return i;
    if (m_Rotations.size() == 1)
        return 0;
    assert(0);
}

unsigned int KeyFrames::getScalingIndex(float animationTime) const {
    for (unsigned int i = 0; i < m_Scalings.size() - 1; i++)
        if (animationTime < m_Scalings[i + 1].timeStamp)
            return i;
    assert(0);
}

glm::mat4 KeyFrames::interpolatePosition(float animationTime) const {
    if (1 == m_Positions.size())
        return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

    unsigned int pOneIndex = getPositionIndex(animationTime);
    unsigned int pTwoIndex = pOneIndex + 1;
    float scaleFactor = getScaleFactor(m_Positions[pOneIndex].timeStamp,
                                       m_Positions[pTwoIndex].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(m_Positions[pOneIndex].position, m_Positions[pTwoIndex].position, scaleFactor);

    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 KeyFrames::interpolateRotation(float animationTime) const {
    if (1 == m_Scalings.size())
        return glm::toMat4(glm::normalize(m_Rotations[0].rotation));

    unsigned int pOneIndex = getRotationIndex(animationTime);
    unsigned int pTwoIndex = pOneIndex + 1;
    float scaleFactor = getScaleFactor(m_Rotations[pOneIndex].timeStamp,
                                       m_Rotations[pTwoIndex].timeStamp, animationTime);
    glm::quat finalRotation = glm::normalize(glm::slerp(m_Rotations[pOneIndex].rotation, m_Rotations[pTwoIndex].rotation, scaleFactor));

    return glm::toMat4(finalRotation);
}

glm::mat4 KeyFrames::interpolateScaling(float animationTime) const {
    if (1 == m_Scalings.size())
        return glm::translate(glm::mat4(1.0f), m_Scalings[0].scaling);

    unsigned int pOneIndex = getScalingIndex(animationTime);
    unsigned int pTwoIndex = pOneIndex + 1;
    float scaleFactor = getScaleFactor(m_Scalings[pOneIndex].timeStamp,
                                       m_Scalings[pTwoIndex].timeStamp, animationTime);
    glm::vec3 finalScaling = glm::mix(m_Scalings[pOneIndex].scaling, m_Scalings[pTwoIndex].scaling, scaleFactor);

    return glm::scale(glm::mat4(1.0f), finalScaling);
}
