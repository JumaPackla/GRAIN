#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};

class triangleRenderer
{
public:
    triangleRenderer() = default;
    triangleRenderer(std::vector<Vertex> mesh_vertices, std::vector<GLuint> mesh_indices);

    triangleRenderer(const triangleRenderer&) = delete;
    triangleRenderer& operator=(const triangleRenderer&) = delete;

    triangleRenderer(triangleRenderer&& other) noexcept;
    triangleRenderer& operator=(triangleRenderer&& other) noexcept;

    ~triangleRenderer();

    int bind() const;

    void draw();

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};
