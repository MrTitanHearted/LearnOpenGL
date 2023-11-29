#include <animator.hpp>

Animator::Animator(Animation* pAnimation) {
    m_CurrentTime = 0.0f;
    m_pCurrentAnimation = pAnimation;

    m_Bones.reserve(200);

    for (unsigned int i = 0; i < 200; i++)
        m_Bones.push_back(glm::mat4(1.0f));
}

void Animator::updateAnimation(float dt) {
    m_DeltaTime = dt;
    if (m_pCurrentAnimation) {
        m_CurrentTime += m_pCurrentAnimation->getTicksPerSecond() * dt;
        m_CurrentTime = fmod(m_CurrentTime, m_pCurrentAnimation->getDuration());
        calculateBoneTransform(&m_pCurrentAnimation->getRootChannel(), glm::mat4(1.0f));
    }
}

void Animator::play(Animation* pAnimation) {
    m_pCurrentAnimation = pAnimation;
    m_CurrentTime = 0.0f;
}

void Animator::calculateBoneTransform(const AnimationChannel* node, glm::mat4 parentTransform) {
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    KeyFrames* keyFrames = m_pCurrentAnimation->findKeyFrames(nodeName);

    if (keyFrames) {
        keyFrames->update(m_CurrentTime);
        nodeTransform = keyFrames->getLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    std::map<std::string, Bone> boneMap = m_pCurrentAnimation->getBoneMap();
    if (boneMap.find(nodeName) != boneMap.end()) {
        int index = boneMap[nodeName].getId();
        glm::mat4 offset = boneMap[nodeName].getOffsetMatrix();
        m_Bones[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->children.size(); i++)
        calculateBoneTransform(&node->children[i], globalTransformation);
}
