#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "app/Application.h"
#include "particles/sphereBody.h"
#include "particles/dustBody.h"

#include "devTools/devTools.h"
#ifdef DEV_DISPLAY
#endif

Application::Application()
{
    initGLFW();
    initGLAD();
    initWindowGUI();
    initRenderShaders();
    initComputeShaders();
    initScene();
}

Application::~Application() 
{
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initGLFW() 
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "GRAIN", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void Application::initGLAD()
{
    if (!gladLoadGL()) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::initWindowGUI()
{
    #ifdef DEV_DISPLAY
        devTools::Manager::Init(window);
    #endif
}

void Application::initRenderShaders()
{
    triangle_render_shader = std::make_unique<Shader>("mesh/triangle_render.vert", "mesh/triangle_render.frag");
    dust_render_shader = std::make_unique<Shader>("dust/render/dust_render.vert", "dust/render/dust_render.frag");
}

void Application::initComputeShaders()
{
    dust_apply_forces_shader = std::make_unique<Shader>("dust/passes/dust_apply_forces.comp");

    dust_upload_shader = std::make_unique<Shader>("dust/render/dust_render_upload.comp");

    dust_cull_count_shader = std::make_unique<Shader>("dust/render/dust_render_cull_count.comp");
    dust_cull_scan_shader = std::make_unique<Shader>("dust/render/dust_render_cull_scan.comp");
    dust_cull_scatter_shader = std::make_unique<Shader>("dust/render/dust_render_cull_scatter.comp");
}

void Application::initScene() 
{
    std::vector<Vertex> vertices = {
        { {-0.8f,  0.5f, 0.5f}, {1,0,0,1} },
        { {-0.4f, -0.5f, 0.0f}, {0,1,0,1} },
        { { 0.0f,  0.5f, 0.0f}, {0,0,1,1} },

        { {0.2f,  0.5f, 0.0f}, {1,1,0,1} },
        { {0.6f, -0.5f, 0.0f}, {0,1,1,1} },
        { {1.0f,  0.5f, 0.0f}, {1,0,1,1} }
    };
    std::vector<GLuint> indices = { 0,1,2, 3,5,4 };

    triangleMesh1 = std::make_unique<triangleRenderer>(vertices, indices);

    sphereBody sphereBody1;
    sphereMesh1 = std::make_unique<sphereRenderer>(sphereBody1, 50, 100, glm::vec4(1, 0, 0, 1));

    std::vector<dustBody> dustParticles;
    int number_of_particles = 3000000;
    dustParticles.reserve(number_of_particles);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    float max_x = 100.0f;
    float min_x = -100.0f;
    float max_y = 10.0f;
    float min_y = -10.0f;
    float max_z = 100.0f;
    float min_z = -100.0f;

    //double min_vel = 0;
    //double max_vel = 1;

    for (int i = 0; i < number_of_particles; i++) {
        dustBody dustParticle;
        dustParticle.position = glm::vec4(
            (min_x + static_cast<float>(rand()) / RAND_MAX * (max_x - min_x)),
            (min_y + static_cast<float>(rand()) / RAND_MAX * (max_y - min_y)),
            (min_z + static_cast<float>(rand()) / RAND_MAX * (max_z - min_z)),
            0.0f
        );
        //dustParticle.velocity = glm::vec4(
        //    min_vel + static_cast<double>(rand()) / RAND_MAX * (max_vel - min_vel),
        //    min_vel + static_cast<double>(rand()) / RAND_MAX * (max_vel - min_vel),
        //    min_vel + static_cast<double>(rand()) / RAND_MAX * (max_vel - min_vel),
        //    0.0f);
        dustParticle.velocity = glm::vec4(0.0f);
        dustParticle.radius = 0.1f;
        dustParticle.mass = 0.1f;
        dustParticles.push_back(dustParticle);
    }

    dustPoints1 = std::make_unique<dustRenderer>(dustParticles);
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app) return;

    app->input.scrollX += static_cast<float>(xoffset);
    app->input.scrollY += static_cast<float>(yoffset);
}

void Application::run() 
{

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        #ifdef DEV_DISPLAY
            devTools::Manager::BeginFrame();
        #endif

        processInput();
        update();
        updateUI();
        render();

        #ifdef DEV_DISPLAY
                devTools::Manager::EndFrame();
        #endif

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    #ifdef DEV_DISPLAY
        devTools::Manager::Shutdown();
    #endif
}

void Application::processInput()
{
    input.moveForward =
        float(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) -
        float(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);

    input.moveRight =
        float(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) -
        float(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);

    input.moveUp =
        float(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) -
        float(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstMouse) {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
    }

    input.mouseDeltaX = float(mouseX - lastMouseX);
    input.mouseDeltaY = float(lastMouseY - mouseY);

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    if (input.scrollY != 0.0f) {
        camera.scroll(input.scrollY);

        input.scrollX = 0.0f;
        input.scrollY = 0.0f;
    }
}

void Application::update()
{
    Time::update(glfwGetTime());
    float dt = Time::deltaTime();

    cameraController.update(camera, input, dt);

    if (!dustPoints1)
        return;

    const GLuint count = static_cast<GLuint>(dustPoints1->getDustCount());
    const GLuint groups = (count + 255) / 256;

    if (dust_apply_forces_shader)
    {
        dust_apply_forces_shader->bind();

        glUniform1f(glGetUniformLocation(dust_apply_forces_shader->getProgram(), "u_DeltaTime"), Time::control(dt));
        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    if (dust_upload_shader)
    {
        dust_upload_shader->bind();

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    }

    if (dust_cull_count_shader && dust_cull_scan_shader && dust_cull_scatter_shader)
    {
        GLuint zero = 0;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, dustPoints1->getTempCountsSSBO());
        glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);

        DrawArraysIndirectCommand resetCmd = {};
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, dustPoints1->getIndirectBuffer());
        glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(resetCmd), &resetCmd);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = float(width) / height;
        glm::mat4 vp = camera.getProjectionMatrix(float(width) / height);

        dust_cull_count_shader->bind();
        glUniformMatrix4fv( glGetUniformLocation(dust_cull_count_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        dust_cull_scan_shader->bind();
        glUniform1ui(glGetUniformLocation(dust_cull_scan_shader->getProgram(), "u_GroupCount"), groups);
        glDispatchCompute(1, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        dust_cull_scatter_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(dust_cull_scatter_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    }
}

void Application::updateUI()
{
    #ifdef DEV_DISPLAY
        devTools::Manager::SetDustCount(static_cast<GLuint>(dustPoints1->getDustCount()));
    #endif

    #ifdef DEV_DISPLAY
        devTools::Manager::SetTimeSpeed(Time::getSpeed());
    #endif

    #ifdef DEV_DISPLAY
        devTools::Manager::SetZoom(camera.getZoom());
    #endif
}

void Application::render()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = float(width) / height;

    glm::mat4 vp = camera.getProjectionMatrix(float(width) / height);

    if (triangle_render_shader and (triangleMesh1 or sphereMesh1))
    {
        triangle_render_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(triangle_render_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));

        if (triangleMesh1) {
            triangleMesh1->draw();
        }
        //if (sphereMesh1) {
        //    sphereMesh1->draw();
        //}
    }

    if (dust_render_shader and (dustPoints1))
    {
        dust_render_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(dust_render_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));

        if (dustPoints1) {
            dustPoints1->draw();
        }
    }
}