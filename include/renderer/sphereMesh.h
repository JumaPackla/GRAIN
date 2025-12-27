#include "glad/glad.h"
#include "glm/glm.hpp"

#include "triangleMesh.h"
#pragma once

class sphereMesh
{
public:
	sphereMesh(float radius, glm::vec3 position, int stacks, int points, glm::vec4 colour);

	void draw();

private:
	float radius;
	glm::vec3 position;
	glm::vec4 colour;
	int stacks;
	int points;
	triangleMesh* mesh;

	void buildMesh();
};