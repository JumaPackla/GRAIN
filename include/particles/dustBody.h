#pragma once

#include <glm/glm.hpp>

struct dustBody {
	glm::vec3 position{ 0 };
	glm::vec3 velocity{ 0 };
	float radius{ 0.1f };
	float mass{ 0.1f };
};