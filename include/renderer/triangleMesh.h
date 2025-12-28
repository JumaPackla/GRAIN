#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};

class triangleMesh
{
public:
    triangleMesh() = default;
    triangleMesh(std::vector<Vertex> mesh_vertices, std::vector<GLuint> mesh_indices);

    triangleMesh(const triangleMesh&) = delete;
    triangleMesh& operator=(const triangleMesh&) = delete;

    triangleMesh(triangleMesh&& other) noexcept;
    triangleMesh& operator=(triangleMesh&& other) noexcept;

    ~triangleMesh();

    int bind() const;

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};
