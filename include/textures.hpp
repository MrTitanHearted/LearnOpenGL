#pragma once

#include <shader.hpp>

#include <map>
#include <vector>
#include <string>

class Parameter {
   public:
    Parameter(unsigned int paramName, unsigned int param) : m_ParamName(paramName), m_Param(param) {}

   protected:
    unsigned int m_ParamName;
    unsigned int m_Param;

    friend class Texture2D;
};

const std::vector<Parameter> TEXTURE_2D_DEFAULT_PARAMS = {
    {0x2802, 0x2901},  // {GL_TEXTURE_WRAP_S, GL_REPEAT},
    {0x2803, 0x2901},  // {GL_TEXTURE_WRAP_T, GL_REPEAT},
    {0x2801, 0x2703},  // {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
    {0x2800, 0x2601},  // {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
};

class Texture2D {
   public:
    Texture2D() {}

    Texture2D(const char *path, std::vector<Parameter> params);
    Texture2D(const std::string &path, std::vector<Parameter> params) : Texture2D(path.c_str(), params) {}

    Texture2D(const char *path) : Texture2D(path, TEXTURE_2D_DEFAULT_PARAMS) {}
    Texture2D(const std::string &path) : Texture2D(path.c_str()) {}

    void bind(const Shader &shader, const char *name, unsigned int index) const;
    void bind(const Shader &shader, const std::string &name, unsigned int index) const {
        bind(shader, name.c_str(), index);
    }

    std::string getPath() const;
    std::string getName() const;
    std::string getDirectory() const;

   private:
    unsigned int m_Id;
    int m_Width;
    int m_Height;
    int m_ChannelsCount;

    std::string m_Path;
    std::string m_Name;
    std::string m_Directory;

    static inline std::map<std::string, Texture2D> s_Cache = {};
};
