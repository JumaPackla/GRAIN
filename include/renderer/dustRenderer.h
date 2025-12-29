#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct dustBody;

class dustRenderer
{
public:
	dustRenderer(const dustBody& body);

	void draw();

private:
	const dustBody& body;
};