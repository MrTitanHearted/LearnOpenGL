#include <animator.hpp>

Animator::Animator(Animation *animation)
{
    CurrentTime = 0.0;
    CurrentAnimation = animation;

    FinalBoneMatrices.reserve(MAX_BONES);

    for (int i = 0; i < MAX_BONES; i++)
        FinalBoneMatrices.push_back(glm::mat4x4(1.0f));
}

void Animator::UpdateAnimation(float dt)
{
    DeltaTime = dt;
    if (CurrentAnimation)
    {
        CurrentTime += CurrentAnimation->GetTicksPerSecond() * dt;
        CurrentTime = fmod(CurrentTime, CurrentAnimation->GetDuration());
        CalculateBoneTransform(&CurrentAnimation->GetRootNode(), glm::mat4x4(1.0f));
    }
}

void Animator::PlayAnimation(Animation *pAnimation)
{
    CurrentAnimation = pAnimation;
    CurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData *node, glm::mat4x4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4x4 nodeTransform = node->transformation;

    Bone* Bone = CurrentAnimation->FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(CurrentTime);
        nodeTransform = Bone->GetLocalTransform();
    }

    glm::mat4x4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4x4 offset = boneInfoMap[nodeName].offset;
        FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}
