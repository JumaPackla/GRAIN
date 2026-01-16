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
    triangle_shader = std::make_unique<Shader>("mesh/triangle_render.vert", "mesh/triangle_render.frag");
    dust_shader = std::make_unique<Shader>("dust/render/dust_render.vert", "dust/render/dust_render.frag");
}

void Application::initComputeShaders()
{
    dust_compute_shader = std::make_unique<Shader>("dust/passes/dust_apply_forces.comp");
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
    int number_of_particles = 100000;
    dustParticles.reserve(number_of_particles);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    float max_x = 10.0f;
    float min_x = -10.0f;
    float max_y = 1.0f;
    float min_y = -1.0f;
    float max_z = 10.0f;
    float min_z = -10.0f;

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
    }

    input.scrollX = 0.0f;
    input.scrollY = 0.0f;
}

void Application::update() 
{
    Time::update(glfwGetTime());
    float dt = Time::deltaTime();

    cameraController.update(camera, input, dt);

    if (dust_compute_shader and (dustPoints1))
    {
        dust_compute_shader->bind();

        glUniform1f(glGetUniformLocation(dust_compute_shader->getProgram(), "u_DeltaTime"), Time::control(dt));

        GLuint count = static_cast<GLuint>(dustPoints1->getDustCount());
        GLuint groups = (count + 255) / 256;
        glDispatchCompute(groups, 1, 1);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
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
}

void Application::render()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = float(width) / height;

    glm::mat4 vp = camera.getProjectionMatrix(float(width) / height);

    if (triangle_shader and (triangleMesh1 or sphereMesh1))
    {
        triangle_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(triangle_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));

        if (triangleMesh1) {
            triangleMesh1->draw();
        }
        //if (sphereMesh1) {
        //    sphereMesh1->draw();
        //}
    }

    if (dust_shader and (dustPoints1))
    {
        dust_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(dust_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));

        if (dustPoints1) {
            dustPoints1->draw();
        }
    }
}