#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <bone.hpp>
#include <animations.hpp>
#include <shader.hpp>

class Animator {
   public:
    Animator(Animation *pAnimation);

    void updateAnimation(float dt);
    void play(Animation *pAnimation);
    void set(const Shader &shader, const char *name) { shader.set(name, m_Bones); }

    const std::vector<glm::mat4> &getBoneMatrices() const { return m_Bones; }

   private:
    std::vector<glm::mat4> m_Bones;
    Animation *m_pCurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;

    void calculateBoneTransform(const AnimationChannel *node, glm::mat4 parentTransform);
};