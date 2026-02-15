#pragma once

struct inputState {
    float moveForward = 0.0f;
    float moveRight = 0.0f;
    float moveUp = 0.0f;
    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;
    float scrollX = 0.0f;
    float scrollY = 0.0f;
    bool togglePause = false;
    bool toggleMouse = false;

    bool mouseCaptured = true;

    bool firstMouse = true;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
};