#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

class Shader {
   public:
    Shader() {}

    Shader(const char *vertexShaderPath, const char *fragmetShaderPath);
    Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
        : Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str()) {}

    // unsigned int getId() { return m_Id; }

    void use() const;

    void setBool(const char *name, bool value) const;
    void setInt(const char *name, int value) const;
    void setFloat(const char *name, float value) const;
    void setVec2(const char *name, float x, float y) const;
    void setVec2(const char *name, const glm::vec2 &value) const;
    void setVec3(const char *name, float x, float y, float z) const;
    void setVec3(const char *name, const glm::vec3 &value) const;
    void setVec4(const char *name, float x, float y, float z, float w) const;
    void setVec4(const char *name, const glm::vec4 &value) const;
    void setMat2(const char *name, const glm::mat2 &value) const;
    void setMat3(const char *name, const glm::mat3 &value) const;
    void setMat4(const char *name, const glm::mat4 &value) const;

    void setBoolArray(const char *name, const void *pValue, unsigned int size) const;
    void setIntArray(const char *name, const void *pValue, unsigned int size) const;
    void setFloatArray(const char *name, const void *pValue, unsigned int size) const;
    void setVec2Array(const char *name, const void *pValue, unsigned int size) const;
    void setVec3Array(const char *name, const void *pValue, unsigned int size) const;
    void setVec4Array(const char *name, const void *pValue, unsigned int size) const;
    void setMat2Array(const char *name, const void *pValue, unsigned int size) const;
    void setMat3Array(const char *name, const void *pValue, unsigned int size) const;
    void setMat4Array(const char *name, const void *pValue, unsigned int size) const;

    void setIntArray(const char *name, const std::vector<int> &value) const { setIntArray(name, (const void *)&value[0], value.size()); }
    void setFloatArray(const char *name, const std::vector<float> &value) const { setFloatArray(name, (const void *)&value[0], value.size()); }
    void setVec2Array(const char *name, const std::vector<glm::vec2> &value) const { setVec2Array(name, (const void *)&value[0], value.size()); }
    void setVec3Array(const char *name, const std::vector<glm::vec3> &value) const { setVec3Array(name, (const void *)&value[0], value.size()); }
    void setVec4Array(const char *name, const std::vector<glm::vec4> &value) const { setVec4Array(name, (const void *)&value[0], value.size()); }
    void setMat2Array(const char *name, const std::vector<glm::mat2> &value) const { setMat2Array(name, (const void *)&value[0], value.size()); }
    void setMat3Array(const char *name, const std::vector<glm::mat3> &value) const { setMat3Array(name, (const void *)&value[0], value.size()); }
    void setMat4Array(const char *name, const std::vector<glm::mat4> &value) const { setMat4Array(name, (const glm::mat4 *)&value[0], value.size()); }

    void set(const char *name, bool value) const { setBool(name, value); }
    void set(const char *name, int value) const { setInt(name, value); }
    void set(const char *name, unsigned int value) const { setInt(name, value); }
    void set(const char *name, float value) const { setFloat(name, value); }
    void set(const char *name, float x, float y) const { setVec2(name, x, y); }
    void set(const char *name, const glm::vec2 &value) const { setVec2(name, value); }
    void set(const char *name, float x, float y, float z) const { setVec3(name, x, y, z); }
    void set(const char *name, const glm::vec3 &value) const { setVec3(name, value); }
    void set(const char *name, float x, float y, float z, float w) const { setVec4(name, x, y, z, w); }
    void set(const char *name, const glm::vec4 &value) const { setVec4(name, value); }
    void set(const char *name, const glm::mat2 &value) const { setMat2(name, value); }
    void set(const char *name, const glm::mat3 &value) const { setMat3(name, value); }
    void set(const char *name, const glm::mat4 &value) const { setMat4(name, value); }

    void set(const char *name, const std::vector<int> &value) const { setIntArray(name, value); }
    void set(const char *name, const std::vector<float> &value) const { setFloatArray(name, value); }
    void set(const char *name, const std::vector<glm::vec2> &value) const { setVec2Array(name, value); }
    void set(const char *name, const std::vector<glm::vec3> &value) const { setVec3Array(name, value); }
    void set(const char *name, const std::vector<glm::vec4> &value) const { setVec4Array(name, value); }
    void set(const char *name, const std::vector<glm::mat2> &value) const { setMat2Array(name, value); }
    void set(const char *name, const std::vector<glm::mat3> &value) const { setMat3Array(name, value); }
    void set(const char *name, const std::vector<glm::mat4> &value) const { setMat4Array(name, value); }

   private:
    unsigned int m_Id;
};
