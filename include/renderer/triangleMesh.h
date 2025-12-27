#include "glad/glad.h"
#include "glm/glm.hpp"

#include "vector"

#pragma once

struct Vertex {
	glm::vec3 position;
	glm::vec4 color;
};

class triangleMesh
{
public:
	triangleMesh(std::vector<Vertex> mesh_vertices, std::vector<GLuint> mesh_indices);
	~triangleMesh();

	int bind();

private:
	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};