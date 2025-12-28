#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class cameraClass
{
public:
	cameraClass();
	~cameraClass();

	void move(const glm::vec3& delta);
	void rotate(float yaw, float pitch);
	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const { return position; }

private:
	glm::vec3 position;
	glm::mat3 rotationMatrix;
	float mouseSensitivity;
};