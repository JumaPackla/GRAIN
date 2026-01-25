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
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    GLFWwindow* window = nullptr;

    Camera camera;
    cameraController cameraController;
    inputState input;

    std::unique_ptr<Shader> triangle_render_shader;
    std::unique_ptr<Shader> dust_render_shader;
    std::unique_ptr<Shader> dust_chunk_render_shader;

    std::unique_ptr<Shader> dust_apply_forces_shader;
    std::unique_ptr<Shader> dust_upload_shader;

    std::unique_ptr<Shader> dust_chunk_cull_shader;
    std::unique_ptr<Shader> dust_particle_scatter_shader;
    std::unique_ptr<Shader> dust_indirect_update_shader;

    std::unique_ptr<triangleRenderer> triangleMesh1;
    std::unique_ptr<sphereRenderer> sphereMesh1;
    std::unique_ptr<dustRenderer> dustPoints1;

    uint32_t frameOffset = 0;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;

    static constexpr uint32_t PARTICLES_PER_CHUNK = 256;  // matches compute shader layout
};
