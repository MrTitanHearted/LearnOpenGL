#include <mesh.hpp>

void BaseMesh::setupBaseMesh()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)offsetof(MeshVertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)offsetof(MeshVertex, TexCoords));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BaseMesh::BaseMesh(const char* name, std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->name = name;

    setupBaseMesh();
}

void BaseMesh::Render(Shader &shader)
{
    unsigned int diffuseNum = 0;
    unsigned int normalNum = 0;
    unsigned int specularNum = 0;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_normal")
            number = std::to_string(normalNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++);
        
        textures[i].texture.bind(&shader, (name + number).c_str(), i);
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void SkinnedMesh::setupSkinnedMesh()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SkinnedMeshVertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedMeshVertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedMeshVertex), (void *)offsetof(SkinnedMeshVertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SkinnedMeshVertex), (void *)offsetof(SkinnedMeshVertex, TexCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, MAX_BONE_INFLUENCE, GL_INT, GL_FALSE, sizeof(SkinnedMeshVertex), (void *)offsetof(SkinnedMeshVertex, BoneIDs));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(SkinnedMeshVertex), (void *)offsetof(SkinnedMeshVertex, Weights));
    glEnableVertexAttribArray(4);
}

SkinnedMesh::SkinnedMesh(const char *name, std::vector<SkinnedMeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->name = name;

    setupSkinnedMesh();
}

void SkinnedMesh::Render(Shader &shader)
{
    unsigned int diffuseNum = 0;
    unsigned int normalNum = 0;
    unsigned int specularNum = 0;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_normal")
            number = std::to_string(normalNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++);
        
        textures[i].texture.bind(&shader, (name + number).c_str(), i);
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
