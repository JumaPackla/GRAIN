#pragma once

#include <memory>

#include "engine/cameraController.h"
#include "engine/inputState.h"
#include "engine/Time.h"
#include "engine/Camera.h"
#include "engine/Shader.h"

#include "renderer/triangleRenderer.h"
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
    void initRenderShaders();
    void initComputeShaders();
    void initScene();
    void processInput();
    void update();
    void render();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window = nullptr;

    Camera camera;
    cameraController cameraController;
    inputState input;

    std::unique_ptr<Shader> triangle_shader;
    std::unique_ptr<Shader> dust_shader;
    std::unique_ptr<Shader> dust_compute_shader;

    std::unique_ptr<triangleRenderer> triangleMesh1;
    std::unique_ptr<sphereRenderer> sphereMesh1;
    std::unique_ptr<dustRenderer> dustPoints1;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
};