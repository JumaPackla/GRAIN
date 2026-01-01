#pragma once

#include <glm/glm.hpp>

struct dustBody {
	glm::vec4 position{ 0 };
	glm::vec4 velocity{ 0 };
	float radius{ 0.1f };
	float mass{ 0.1f };
	float _pad0;
	float _pad1;
};