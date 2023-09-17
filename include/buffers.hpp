#pragma once

class RenderBuffer
{
private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    unsigned int num_indices;

public:
    RenderBuffer(): vao(0), vbo(0), ebo(0), num_indices(0) {}
    RenderBuffer(const void *vertices, unsigned int vertices_size, const void *indices, unsigned int indices_size);

    ~RenderBuffer();

    unsigned int GetVAO() { return vao; }
    unsigned int GetVBO() { return vbo; }
    unsigned int GetEBO() { return ebo; }
    unsigned int GetNumIndices() { return num_indices; }

    void LinkAttribute(unsigned int layout, unsigned int size, unsigned int type, int stride, void *offset);

    void Render();
    void RenderArrays(int num);
};