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
<<<<<<< HEAD
    triangle_render_shader = std::make_unique<Shader>("mesh/triangle_render.vert", "mesh/triangle_render.frag");
    dust_render_shader = std::make_unique<Shader>("dust/render/dust_render.vert", "dust/render/dust_render.frag");
    dust_chunk_render_shader = std::make_unique<Shader>("dust/render/dust_render_chunk.vert", "dust/render/dust_render_chunk.frag");

    dust_upload_shader = std::make_unique<Shader>("dust/render/dust_render_upload.comp");

    dust_chunk_cull_shader = std::make_unique<Shader>("dust/render/dust_render_chunk_cull.comp");
    dust_particle_scatter_shader = std::make_unique<Shader>("dust/render/dust_render_particle_scatter.comp");

    dust_indirect_update_shader = std::make_unique<Shader>("dust/render/dust_render_indirect_update.comp");
=======
    triangle_shader = std::make_unique<Shader>("mesh/triangle_render.vert", "mesh/triangle_render.frag");
    dust_shader = std::make_unique<Shader>("dust/render/dust_render.vert", "dust/render/dust_render.frag");
>>>>>>> parent of df91cb2 (Culling)
}

void Application::initComputeShaders()
{
<<<<<<< HEAD
    dust_apply_forces_shader = std::make_unique<Shader>("dust/passes/dust_apply_forces.comp");
=======
    dust_compute_shader = std::make_unique<Shader>("dust/passes/dust_apply_forces.comp");
>>>>>>> parent of df91cb2 (Culling)
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
    int number_of_particles = 1000000;
=======
    int number_of_particles = 100000;
>>>>>>> parent of df91cb2 (Culling)
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

<<<<<<< HEAD
    if (!dustPoints1)
        return;

    const GLuint count = static_cast<GLuint>(dustPoints1->getDustCount());
    const GLuint groups = (count + 255) / 256;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glm::mat4 vp = camera.getProjectionMatrix(float(width) / height);

    // Step 1: Apply physics forces
    if (dust_apply_forces_shader)
    {
        dust_apply_forces_shader->bind();
        glUniform1f(glGetUniformLocation(dust_apply_forces_shader->getProgram(), "u_DeltaTime"), Time::control(dt));
=======
    if (dust_compute_shader and (dustPoints1))
    {
        dust_compute_shader->bind();

        glUniform1f(glGetUniformLocation(dust_compute_shader->getProgram(), "u_DeltaTime"), Time::control(dt));

        GLuint count = static_cast<GLuint>(dustPoints1->getDustCount());
        GLuint groups = (count + 255) / 256;
>>>>>>> parent of df91cb2 (Culling)
        glDispatchCompute(groups, 1, 1);

        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
<<<<<<< HEAD

    // Step 2: Upload sim positions to render buffer
    if (dust_upload_shader)
    {
        dust_upload_shader->bind();
        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    // Step 3: Clear counters
    GLuint zero = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, dustPoints1->getVisibleCountSSBO());
    glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, dustPoints1->getChunkVisibleSSBO());
    glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // Step 4: Frustum cull chunks (per-chunk, not per-particle!)
    if (dust_chunk_cull_shader)
    {
        const GLuint chunkGroups = static_cast<GLuint>(dustPoints1->getChunkCount());
        dust_chunk_cull_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(dust_chunk_cull_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glDispatchCompute(chunkGroups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    // Step 5: Per-particle culling and LOD within visible chunks
    if (dust_particle_scatter_shader)
    {
        dust_particle_scatter_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(dust_particle_scatter_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glUniform3fv(glGetUniformLocation(dust_particle_scatter_shader->getProgram(), "u_CameraPos"), 1, glm::value_ptr(camera.getPosition()));
        glUniform1f(glGetUniformLocation(dust_particle_scatter_shader->getProgram(), "u_MaxDistance"), 1000.0f);
        glUniform1f(glGetUniformLocation(dust_particle_scatter_shader->getProgram(), "u_FarDistance"), 500.0f);
        glUniform1ui(glGetUniformLocation(dust_particle_scatter_shader->getProgram(), "u_FrameOffset"), frameOffset);
        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    // Step 6: Update indirect draw command
    if (dust_indirect_update_shader)
    {
        dust_indirect_update_shader->bind();
        glDispatchCompute(1, 1, 1);
        glMemoryBarrier(GL_COMMAND_BARRIER_BIT);
    }

    frameOffset = (frameOffset + 1) % 4;
=======
>>>>>>> parent of df91cb2 (Culling)
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
    glm::mat4 vp = camera.getProjectionMatrix(float(width) / height);

<<<<<<< HEAD
    if (triangle_render_shader && (triangleMesh1 || sphereMesh1))
=======
    if (triangle_shader and (triangleMesh1 or sphereMesh1))
>>>>>>> parent of df91cb2 (Culling)
    {
        triangle_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(triangle_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));

        if (triangleMesh1)
            triangleMesh1->draw();

        if (sphereMesh1)
            sphereMesh1->draw();
    }

<<<<<<< HEAD
    if (dust_render_shader && dustPoints1)
    {
        dust_render_shader->bind();
=======
    if (dust_shader and (dustPoints1))
    {
        dust_shader->bind();
        glUniformMatrix4fv(glGetUniformLocation(dust_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
>>>>>>> parent of df91cb2 (Culling)

        glUniformMatrix4fv(glGetUniformLocation(dust_render_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glUniform3fv(glGetUniformLocation(dust_render_shader->getProgram(), "u_CameraPos"), 1, glm::value_ptr(camera.getPosition()));
        glUniform1f(glGetUniformLocation(dust_render_shader->getProgram(), "u_LodNear"), 300.0f);
        glUniform1f(glGetUniformLocation(dust_render_shader->getProgram(), "u_LodFar"), 600.0f);

        dustPoints1->drawParticles();
    }

    if (dust_chunk_render_shader && dustPoints1)
    {
        dust_chunk_render_shader->bind();

        glUniformMatrix4fv(glGetUniformLocation(dust_chunk_render_shader->getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));
        glUniform3fv(glGetUniformLocation(dust_chunk_render_shader->getProgram(), "u_CameraPos"), 1, glm::value_ptr(camera.getPosition()));
        glUniform1f(glGetUniformLocation(dust_chunk_render_shader->getProgram(), "u_LodNear"), 300.0f);
        glUniform1f(glGetUniformLocation(dust_chunk_render_shader->getProgram(), "u_LodFar"), 600.0f);
        glUniform1f(glGetUniformLocation(dust_chunk_render_shader->getProgram(), "u_PointScale"), 10.0f);

        dustPoints1->drawChunks();
    }
}