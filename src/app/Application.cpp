#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "app/Application.h"
#include "app/GLFWInputAdaptor.h"
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
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (!app) return;
        if (app->inputState.mouseCaptured) {
            app->inputState.scrollX += static_cast<float>(xoffset);
            app->inputState.scrollY += static_cast<float>(yoffset);
        }
        });
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

    dust_render_upload_shader = std::make_unique<Shader>("dust/render/dust_render_upload.comp");

    dust_render_cull_count_shader = std::make_unique<Shader>("dust/render/dust_render_cull_count.comp");
    dust_render_cull_scan_shader = std::make_unique<Shader>("dust/render/dust_render_cull_scan.comp");
    dust_render_cull_scatter_shader = std::make_unique<Shader>("dust/render/dust_render_cull_scatter.comp");
}

void Application::initComputeShaders()
{
<<<<<<< HEAD
    dustSim.setShader(std::make_unique<Shader>("dust/barnes-hut/gravity.comp"), std::make_unique<Shader>("dust/passes/dust_leapfrog_init.comp"), std::make_unique<Shader>("dust/passes/dust_leapfrog_step.comp"),
                      std::make_unique<Shader>("dust/barnes-hut/morton.comp"), std::make_unique<Shader>("dust/barnes-hut/radix_histogram.comp"), std::make_unique<Shader>("dust/barnes-hut/radix_scan.comp"),
                      std::make_unique<Shader>("dust/barnes-hut/radix_scatter.comp"), std::make_unique<Shader>("dust/barnes-hut/lvbh_init_leaves.comp"), std::make_unique<Shader>("dust/barnes-hut/lvbh_build.comp"),
                      std::make_unique<Shader>("dust/barnes-hut/lvbh_mass.comp"));
=======
    dustSim.setShader(std::make_unique<Shader>("dust/forces/gravity.compinc"), std::make_unique<Shader>("dust/passes/dust_integrate.comp"));
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)
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

    //sphereBody sphereBody1;
    //sphereMesh1 = std::make_unique<sphereRenderer>(sphereBody1, 50, 100, glm::vec4(1, 0, 0, 1));

    std::vector<dustBody> dustParticles;
<<<<<<< HEAD
    int particleCount = 5000;
    dustParticles.reserve(particleCount);
=======
    int number_of_particles = 10000;
    dustParticles.reserve(number_of_particles);
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)

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
        dustParticle.acceleration = glm::vec4(0.0f);
        dustParticle.radius = 0.1f;
        dustParticle.mass = 0.1f;
        dustParticles.push_back(dustParticle);
    }

<<<<<<< HEAD
    dustSim.init(dustParticles.size(), dustParticles.data());
    dustPoints1 = std::make_unique<dustRenderer>(dustSim);
=======
    dustPoints1 = std::make_unique<dustRenderer>(dustParticles);
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
<<<<<<< HEAD
    GLFWInputAdapter::pollInput(window, inputState);

    if (inputState.togglePause)
        Time::togglePause();
=======
    input.moveForward =
        float(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) -
        float(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)

    if (inputState.scrollY != 0.0f)
    {
        camera.scroll(inputState.scrollY);
        inputState.scrollX = 0.0f;
        inputState.scrollY = 0.0f;
    }
}

void Application::update()
{
    Time::update(glfwGetTime());
<<<<<<< HEAD

    cameraController.update(camera, inputState, Time::deltaTime());

    dustSim.update(Time::simDeltaTime());
=======
    float dt = Time::deltaTime();

    cameraController.update(camera, input, dt);

    if (!dustPoints1)
        return;

    const GLuint count = static_cast<GLuint>(dustPoints1->getDustCount());
    const GLuint groups = (count + 255) / 256;

    //if (gravity_shader)
    //{
    //    gravity_shader->bind();

    //    glUniform1f(glGetUniformLocation(gravity_shader->getProgram(), "u_Softening"), 0.01);
    //    glUniform1f(glGetUniformLocation(gravity_shader->getProgram(), "u_MaxAccel"), 1000.0);
    //    glDispatchCompute(groups, 1, 1);
    //    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
    //}

    //if (dust_integrate_shader)
    //{
    //    dust_integrate_shader->bind();

    //    glUniform1f(glGetUniformLocation(dust_integrate_shader->getProgram(), "u_DeltaTime"), Time::control(dt));
    //    glDispatchCompute(groups, 1, 1);
    //    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //}

    dustSim.update(dt);
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)

    if (dust_render_upload_shader)
    {
        dust_render_upload_shader->bind();

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    }

<<<<<<< HEAD
    /*if (dust_render_cull_count_shader && dust_render_cull_scan_shader && dust_render_cull_scatter_shader)
=======
 /*   if (dust_render_cull_count_shader && dust_render_cull_scan_shader && dust_render_cull_scatter_shader)
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)
    {
        GLuint count = static_cast<GLuint>(dustPoints1->getDustCount());
        GLuint groups = (count + 255) / 256;

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

        dust_render_cull_count_shader->bind();
        glUniformMatrix4fv( glGetUniformLocation(dust_render_cull_count_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        dust_render_cull_scan_shader->bind();
        glUniform1ui(glGetUniformLocation(dust_render_cull_scan_shader->getProgram(), "u_GroupCount"), groups);
        glDispatchCompute(1, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        dust_render_cull_scatter_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(dust_render_cull_scatter_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    }*/
}

void Application::updateUI()
{
    #ifdef DEV_DISPLAY
        devTools::Manager::SetDustCount(static_cast<GLuint>(dustPoints1->getDustCount()));
    #endif

    #ifdef DEV_DISPLAY
        devTools::Manager::SetTimeSpeed(Time::getSimSpeed());
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
        if (sphereMesh1) {
            sphereMesh1->draw();
        }
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