#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

class BoneInfo {
   public:
    BoneInfo() = default;

    BoneInfo(int id, glm::mat4 offsetMatrix)
        : m_Id(id), m_OffsetMatrix(offsetMatrix) {}

    unsigned int getId() const { return m_Id; }
    const glm::mat4 &getOffsetMatrix() { return m_OffsetMatrix; }

   private:
    unsigned int m_Id;
    glm::mat4 m_OffsetMatrix;
};

class Bone {
    //    public:

    //    private:
    //     unsigned int m_Id;
    //     std::string m_Name;
    //     std::vector<Bone> m_Children;
};