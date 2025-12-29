#pragma once

#include <memory>

#include "engine/cameraController.h"
#include "engine/inputState.h"
#include "engine/Time.h"
#include "engine/Camera.h"
#include "engine/shaderPipeline.h"

#include "renderer/triangleMesh.h"
#include "renderer/sphereRenderer.h"
#include "renderer/dustRenderer.h"

struct GLFWwindow;

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    void initGLFW();
    void initScene();
    void processInput();
    void update(float dt);
    void render();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window = nullptr;

    Camera camera;
    cameraController cameraController;
    inputState input;

    std::unique_ptr<shaderPipeline> shader;

    std::unique_ptr<triangleMesh> triangleCombinedMesh1;
    std::unique_ptr<sphereRenderer> sphereMesh1;
    std::unique_ptr<dustRenderer> dustPoint1;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
};