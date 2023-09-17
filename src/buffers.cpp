#include <buffers.hpp>
#include <glad/glad.h>

RenderBuffer::RenderBuffer(const void *vertices, unsigned int vertices_size, const void *indices, unsigned int indices_size)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);

    num_indices = indices_size / sizeof(unsigned int);
}

RenderBuffer::~RenderBuffer()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void RenderBuffer::LinkAttribute(unsigned int layout, unsigned int size, unsigned int type, int stride, void *offset)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderBuffer::Render()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void RenderBuffer::RenderArrays(int num)
{
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, num);

    glBindVertexArray(0);
}
