#include <glm/glm.hpp>

#include "engine/cameraController.h"

cameraController::cameraController(float speed, float sensitivity) : speed(speed), sensitivity(sensitivity) {}

void cameraController::update(Camera& camera, const inputState& input, float deltaTime) const
{
    glm::vec3 moveDir(0.0f);

    moveDir.z -= input.moveForward;
    moveDir.x += input.moveRight;
    moveDir.y += input.moveUp;

    camera.move(moveDir * speed * deltaTime);
    camera.rotate(-input.mouseDeltaX * sensitivity, input.mouseDeltaY * sensitivity);
}
