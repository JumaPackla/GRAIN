#include "engine/cameraClass.h"

cameraClass::cameraClass()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotationMatrix = glm::mat3(1.0f);
	mouseSensitivity = 0.1f;
}

cameraClass::~cameraClass()
{
}

void cameraClass::move(const glm::vec3& delta)
{
	position += rotationMatrix * delta;

	glm::vec3 forward(rotationMatrix[2]);
	glm::vec3 right(rotationMatrix[0]);

	forward.y = 0.0f;
	right.y = 0.0f;

	forward = glm::normalize(forward);
	right = glm::normalize(right);

	position += right * delta.x;
	position += forward * delta.z;
	position.y += delta.y;
}

void cameraClass::rotate(float yaw, float pitch)
{
	glm::mat4 yawMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(yaw * mouseSensitivity), glm::vec3(0, 1, 0));
	glm::mat4 pitchMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(pitch * mouseSensitivity), glm::vec3(1, 0, 0));

	rotationMatrix = glm::mat3(yawMatrix * pitchMatrix) * rotationMatrix;
}

glm::mat4 cameraClass::getViewMatrix() const
{
	return glm::lookAt(position, position + rotationMatrix * glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
}