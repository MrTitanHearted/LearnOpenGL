#include <render_buffer.hpp>

#include <glad/glad.h>

VertexDescriptor::VertexDescriptor(unsigned int location,
                                   unsigned int size,
                                   unsigned int type,
                                   unsigned int stride,
                                   unsigned int offset)
    : location(location), size(size), type(type) {
    switch (type) {
        case GL_FLOAT:
        case GL_INT:
        case GL_UNSIGNED_INT: {
            this->stride = stride * sizeof(float);
            this->offset = offset * sizeof(float);
            break;
        }
        case GL_DOUBLE: {
            this->stride = stride * sizeof(double);
            this->offset = offset * sizeof(double);
            break;
        }
        default:
            std::cerr << "[ERROR]: There is no such type with handle: " << type << std::endl;
    };
}

RenderBuffer::RenderBuffer(const void *vertices,
                           int verticesSize,
                           const unsigned int *indices,
                           int indicesSize,
                           std::vector<VertexDescriptor> vertexDescriptors) {
    m_IndicesCount = indicesSize / sizeof(unsigned int);

    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_Vbo);
    glGenBuffers(1, &m_Ebo);

    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    for (const VertexDescriptor &vertexDescriptor : vertexDescriptors) {
        glVertexAttribPointer(
            vertexDescriptor.location,
            vertexDescriptor.size,
            vertexDescriptor.type,
            GL_FALSE,
            vertexDescriptor.stride,
            (void *)vertexDescriptor.offset);
        glEnableVertexAttribArray(vertexDescriptor.location);

        m_VerticesCount = verticesSize / vertexDescriptor.stride;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderBuffer::render() const {
    glBindVertexArray(m_Vao);
    glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, nullptr);
}

void RenderBuffer::renderVertices() const {
    glBindVertexArray(m_Vao);
    glDrawArrays(GL_TRIANGLES, 0, m_VerticesCount);
}
