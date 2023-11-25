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
        case GL_FLOAT_VEC2: {
            this->stride = stride * sizeof(float) * 2;
            this->offset = offset * sizeof(float) * 2;
            break;
        }
        case GL_FLOAT_VEC3: {
            this->stride = stride * sizeof(float) * 3;
            this->offset = offset * sizeof(float) * 3;
            break;
        }
        case GL_FLOAT_VEC4: {
            this->stride = stride * sizeof(float) * 4;
            this->offset = offset * sizeof(float) * 4;
            break;
        }
        case GL_DOUBLE_VEC2: {
            this->stride = stride * sizeof(double) * 2;
            this->offset = offset * sizeof(double) * 2;
            break;
        }
        case GL_DOUBLE_VEC3: {
            this->stride = stride * sizeof(double) * 3;
            this->offset = offset * sizeof(double) * 3;
            break;
        }
        case GL_DOUBLE_VEC4: {
            this->stride = stride * sizeof(double) * 4;
            this->offset = offset * sizeof(double) * 4;
            break;
        }
        case GL_FLOAT_MAT2: {
            this->stride = stride * sizeof(float) * 4;
            this->offset = offset * sizeof(float) * 4;
            break;
        }
        case GL_FLOAT_MAT3: {
            this->stride = stride * sizeof(float) * 9;
            this->offset = offset * sizeof(float) * 9;
            break;
        }
        case GL_FLOAT_MAT4: {
            this->stride = stride * sizeof(float) * 16;
            this->offset = offset * sizeof(float) * 16;
            break;
        }
        case GL_DOUBLE_MAT2: {
            this->stride = stride * sizeof(double) * 4;
            this->offset = offset * sizeof(double) * 4;
            break;
        }
        case GL_DOUBLE_MAT3: {
            this->stride = stride * sizeof(double) * 9;
            this->offset = offset * sizeof(double) * 9;
            break;
        }
        case GL_DOUBLE_MAT4: {
            this->stride = stride * sizeof(double) * 16;
            this->offset = offset * sizeof(double) * 16;
            break;
        }

        default:
            std::cerr << "[ERROR]: There is no such type or supported one with handle: " << type << std::endl;
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
