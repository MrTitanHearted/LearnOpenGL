#pragma once
#include <shaders.hpp>

class Texture2D
{
private:
    unsigned int id;
    int width;
    int height;
    int nrChannels;
public:
    Texture2D() {}
    Texture2D(const char* path);

    void bind(const Shader *shader, const std::string &name, int value) const;
    int get_width() const;
    int get_height() const;
    int get_numChannels() const;
};