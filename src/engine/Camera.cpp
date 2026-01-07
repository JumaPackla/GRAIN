#include "engine/Camera.h"

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    mouseSensitivity = 0.1f;
    updateVectors();
}

Camera::~Camera() {}

void Camera::move(const glm::vec3& delta)
{
    position += right * delta.x;
    position += up * delta.y;
    position += forward * delta.z;
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    yaw += deltaYaw * mouseSensitivity;
    pitch += deltaPitch * mouseSensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();
}

void Camera::updateVectors()
{
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(f);

    right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    up = glm::normalize(glm::cross(right, forward));
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + forward, up);
}