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

#include "simulation/dustSim.h"


struct GLFWwindow;

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    void initGLFW();
    void initGLAD();
    void initWindowGUI();
    void initRenderShaders();
    void initComputeShaders();
    void initScene();
    void processInput();
    void update();
    void updateUI();
    void render();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window = nullptr;

    Camera camera;
    cameraController cameraController;
    inputState inputState;
    dustSimulation dustSim;

    std::unique_ptr<Shader> triangle_render_shader;
    std::unique_ptr<Shader> dust_render_shader;
    std::unique_ptr<Shader> dust_render_upload_shader;
    std::unique_ptr<Shader> dust_render_cull_count_shader;
    std::unique_ptr<Shader> dust_render_cull_scan_shader;
    std::unique_ptr<Shader> dust_render_cull_scatter_shader;

    std::unique_ptr<triangleRenderer> triangleMesh1;
    std::unique_ptr<sphereRenderer> sphereMesh1;
    std::unique_ptr<dustRenderer> dustPoints1;
};