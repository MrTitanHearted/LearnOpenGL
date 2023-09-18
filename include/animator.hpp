#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <animation.hpp>
#include <bone.hpp>

#ifndef MAX_BONES
#define MAX_BONES 100
#endif

class Animator
{
private:
    std::vector<glm::mat4x4> FinalBoneMatrices;
    Animation* CurrentAnimation;
    float CurrentTime;
    float DeltaTime;
public:
    Animator(Animation* animation);
    void UpdateAnimation(float dt);
    void PlayAnimation(Animation* pAnimation);
    void CalculateBoneTransform(const AssimpNodeData *node, glm::mat4x4 parentTransform);
    std::vector<glm::mat4x4> GetFinalMatrices() { return FinalBoneMatrices; }
};