#pragma once

#include "Camera.h"
#include "inputState.h"

class cameraController {
public:
    cameraController(float speed = 3.0f, float sensitivity = 1.0f);

    void update(Camera& camera, const inputState& input, float deltaTime) const;

private:
    float speed;
    float sensitivity;
};
