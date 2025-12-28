#pragma once

#include <glm/glm.hpp>

#include "triangleMesh.h"

struct sphereBody;

class sphereRenderer
{
public:
	sphereRenderer(const sphereBody& body, int stacks, int points, glm::vec4 colour);

	void draw();

private:
	const sphereBody& body;
	glm::vec4 colour;
	triangleMesh mesh;

	void buildMesh(int stacks, int points);
};