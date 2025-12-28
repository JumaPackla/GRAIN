#include "renderer/triangleMesh.h"

triangleMesh::triangleMesh(std::vector<Vertex> mesh_vertices, std::vector<GLuint> mesh_indices) : vertices(std::move(mesh_vertices)), indices(std::move(mesh_indices))
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (!vertices.empty()) {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    if (!indices.empty()) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

triangleMesh::triangleMesh(triangleMesh&& other) noexcept
    : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO),
    vertices(std::move(other.vertices)),
    indices(std::move(other.indices))
{
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

triangleMesh& triangleMesh::operator=(triangleMesh&& other) noexcept
{
    if (this != &other) {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);

        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);

        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }
    return *this;
}

triangleMesh::~triangleMesh() 
{
    if (EBO) glDeleteBuffers(1, &EBO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

int triangleMesh::bind() const 
{
    glBindVertexArray(VAO);
    return static_cast<int>(indices.size());
}

void triangleMesh::draw() 
{
    int indexCount = bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}