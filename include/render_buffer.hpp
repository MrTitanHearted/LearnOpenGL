#pragma once
#include <vector>
#include <iostream>

class VertexDescriptor {
   protected:
    unsigned int location;
    unsigned int size;
    unsigned int type;
    unsigned int stride;
    unsigned int offset;

    friend class RenderBuffer;

   public:
    VertexDescriptor(unsigned int location,
                     unsigned int size,
                     unsigned int type,
                     unsigned int stride,
                     unsigned int offset);
};

class RenderBuffer {
   public:
    RenderBuffer() {}

    RenderBuffer(const void *vertices,
                 int verticesSize,
                 const unsigned int *indices,
                 int indicesSize,
                 std::vector<VertexDescriptor> vertexDescriptors);

    template <typename Vertex>
    RenderBuffer(const std::vector<Vertex> &vertices,
                 const std::vector<unsigned int> &indices,
                 std::vector<VertexDescriptor> vertexDescriptors)
        : RenderBuffer(
              vertices.data(),
              vertices.size() * sizeof(Vertex),
              indices.data(),
              indices.size() * sizeof(unsigned int),
              vertexDescriptors) {}

    void render() const;
    void renderVertices() const;

   private:
    unsigned int m_Vao;
    unsigned int m_Vbo;
    unsigned int m_Ebo;

    unsigned int m_IndicesCount;
    unsigned int m_VerticesCount;
};