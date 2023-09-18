#include <bone.hpp>

Bone::Bone(const std::string &name, int ID, const aiNodeAnim *channel)
{
    this->Name = name;
    this->ID = ID;
    this->LocalTransform = glm::mat4x4(1.0f);

    NumPositions = channel->mNumPositionKeys;
    for (int positionIndex = 0; positionIndex < NumPositions; positionIndex++)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
        data.timeStamp = timeStamp;
        Positions.push_back(data);
    }

    NumRotations = channel->mNumRotationKeys;
    for (int rotationIndex = 0; rotationIndex < NumRotations; rotationIndex++)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
        data.timeStamp = timeStamp;
        Rotations.push_back(data);
    }

    NumScalings = channel->mNumScalingKeys;
    for (int scalingIndex = 0; scalingIndex < NumScalings; scalingIndex++)
    {
        aiVector3D aiScaling = channel->mScalingKeys[scalingIndex].mValue;
        float timeStamp = channel->mScalingKeys[scalingIndex].mTime;
        KeyScaling data;
        data.scale = AssimpGLMHelpers::GetGLMVec(aiScaling);
        data.timeStamp = timeStamp;
        Scalings.push_back(data);
    }
}

void Bone::Update(float animationTime)
{
    glm::mat4x4 translation = InterpolatePosition(animationTime);
    glm::mat4x4 rotation = InterpolateRotation(animationTime);
    glm::mat4x4 scaling = InterpolateScaling(animationTime);
    LocalTransform = translation * rotation * scaling;
}

int Bone::GetPositionIndex(float animationTime)
{
    for (int index = 0; index < NumPositions - 1; index++)
    {
        if (animationTime < Positions[index + 1].timeStamp)
            return index;
    }
    assert(0);
}

int Bone::GetRotationIndex(float animationTime)
{
    for (int index = 0; index < NumRotations - 1; index++)
    {
        if (animationTime < Rotations[index + 1].timeStamp)
            return index;
    }
    assert(0);
}

int Bone::GetScalingIndex(float animationTime)
{
    for (int index = 0; index < NumScalings - 1; index ++)
    {
        if (animationTime < Scalings[index + 1].timeStamp)
            return index;
    }
    assert(0);
}

float Bone::GetScalingFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scalingFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scalingFactor = midWayLength / framesDiff;
    return scalingFactor;
}

glm::mat4x4 Bone::InterpolatePosition(float animationTime)
{
    if (1 == NumPositions)
        return glm::translate(glm::mat4x4(1.0f), Positions[0].position);
    
    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float scalingFactor = GetScalingFactor(Positions[p0Index].timeStamp, Positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(Positions[p0Index].position, Positions[p1Index].position, scalingFactor);
    return glm::translate(glm::mat4x4(1.0f), finalPosition);
}

glm::mat4x4 Bone::InterpolateRotation(float animationTime)
{
    if (1 == NumRotations)
    {
        auto rotation = glm::normalize(Rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float scalingFactor = GetScalingFactor(Rotations[p0Index].timeStamp, Rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(Rotations[p0Index].orientation, Rotations[p1Index].orientation, scalingFactor);
    return glm::toMat4(glm::normalize(finalRotation));
}

glm::mat4x4 Bone::InterpolateScaling(float animationTime)
{
    if (1 == NumScalings)
        return glm::scale(glm::mat4x4(1.0f), Scalings[0].scale);
    
    int p0Index = GetScalingIndex(animationTime);
    int p1Index = p0Index + 1;
    float scalingFactor = GetScalingFactor(Scalings[p0Index].timeStamp, Scalings[p1Index].timeStamp, animationTime);
    glm::vec3 finalScaling = glm::mix(Scalings[p0Index].scale, Scalings[p1Index].scale, scalingFactor);
    return glm::scale(glm::mat4x4(1.0f), finalScaling);
}
