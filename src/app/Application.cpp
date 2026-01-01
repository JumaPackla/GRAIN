#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "app/Application.h"
#include "particles/sphereBody.h"
#include "particles/dustBody.h"

Application::Application()
{
    initGLFW();
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

    window = glfwCreateWindow(800, 600, "GRAIN", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGL()) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void Application::initRenderShaders()
{
    triangle_shader = std::make_unique<Shader>("shaders/mesh/triangle_vertex_shader.vert", "shaders/mesh/triangle_fragment_shader.frag");
    dust_shader = std::make_unique<Shader>("shaders/dust/dust_vertex_shader.vert", "shaders/dust/dust_fragment_shader.frag");
}

void Application::initComputeShaders()
{
    dust_compute_shader = std::make_unique<Shader>("shaders/dust/dust_compute_shader.comp");
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

    //dustBody dustBody1;
    //dustPoints1 = std::make_unique<dustRenderer>(std::vector<dustBody>{dustBody1});

    std::vector<dustBody> dustParticles;
    dustParticles.reserve(1000);

    int n = 10;
    float spacing = 0.2f;
    for (size_t x = 0; x < n; ++x) {
        for (size_t y = 0; y < n; ++y) {
            for (size_t z = 0; z < n; ++z) {
                dustBody d;
                d.position = glm::vec4(
                    (x - n / 2) * spacing,
                    (y - n / 2) * spacing,
                    -(z * spacing + 2.0f),
                    1.0f
                );
                d.velocity = glm::vec4(0.0f);
                d.radius = 0.1f;
                d.mass = 0.1f;
                dustParticles.push_back(d);
            }
        }
    }

    dustPoints1 = std::make_unique<dustRenderer>(dustParticles);
}

void Application::run() 
{
    while (!glfwWindowShouldClose(window)) {
        Time::update(glfwGetTime());
        float dt = Time::deltaTime();

        processInput();
        update(dt);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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
}

void Application::update(float dt) 
{
    cameraController.update(camera, input, dt);

    if (dust_compute_shader and (dustPoints1))
    {
        dust_compute_shader->bind();

        glUniform1f(glGetUniformLocation(dust_compute_shader->getProgram(), "u_DeltaTime"), dt);

        GLuint count = static_cast<GLuint>(dustPoints1->getDustCount());
        GLuint groups = (count + 255) / 256;
        glDispatchCompute(groups, 1, 1);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
}

void Application::render()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = float(width) / height;

    glm::mat4 vp = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f) * camera.getViewMatrix();

    glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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