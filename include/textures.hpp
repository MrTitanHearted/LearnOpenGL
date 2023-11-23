#pragma once

#include <shaders.hpp>

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
    Texture2D(const char *path, std::vector<Parameter> params);
    Texture2D(const std::string &path, std::vector<Parameter> params) : Texture2D(path.c_str(), params) {}

    Texture2D(const char *path) : Texture2D(path, TEXTURE_2D_DEFAULT_PARAMS) {}
    Texture2D(const std::string &path) : Texture2D(path.c_str()) {}

    void activate(const Shader &shader, const char *name, unsigned int index) const;
    void activate(const Shader &shader, const std::string &name, unsigned int index) const {
        activate(shader, name.c_str(), index);
    }

   private:
    unsigned int m_Id;
    int m_Width;
    int m_Height;
    int m_ChannelsCount;
};
