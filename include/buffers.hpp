#pragma once

class RenderBuffer
{
private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    unsigned int num_indices;

public:
    RenderBuffer(const void *vertices, unsigned int vertices_size, const void *indices, unsigned int indices_size);

    ~RenderBuffer();

    void linkAttribute(unsigned int layout, unsigned int size, unsigned int type, int stride, void *offset);

    void render();
    void render_arrays(int num);
};