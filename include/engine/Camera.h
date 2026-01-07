#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Camera
{
public:
    Camera();
    ~Camera();

    void move(const glm::vec3& delta);
    void rotate(float deltaYaw, float deltaPitch);

    glm::mat4 getViewMatrix() const;

private:
    void updateVectors();

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    float yaw;
    float pitch;

    float mouseSensitivity;
};