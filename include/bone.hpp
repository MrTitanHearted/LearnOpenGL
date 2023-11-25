#pragma once

#include <string>
#include <vector>

class Bone {
   public:
   private:
    unsigned int m_Id;
    std::string m_Name;
    std::vector<Bone> m_Children;
};