#pragma once
#include <glm/glm.hpp>

struct sphereBody {
	glm::vec3 position{ 0 };
	glm::vec3 velocity{ 0 };
	float radius{ 1.0f };
	float mass{ 1.0f };
};