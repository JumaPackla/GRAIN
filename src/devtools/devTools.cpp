#include "devtools/devTools.h"
#ifdef DEV_DISPLAY

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdio>

namespace devTools {

    static GLFWwindow* g_Window = nullptr;
    int devTools::Manager::s_dustCount = 0;
    float devTools::Manager::s_timeSpeed = 1.0f;

    void Manager::Init(GLFWwindow* window) {
        g_Window = window;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void Manager::SetDustCount(int dustCount) {
        s_dustCount = dustCount;
    }

    void Manager::SetTimeSpeed(float timeSpeed) {
        s_timeSpeed = timeSpeed;
    }

    void Manager::BeginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(10, 10));

        ImGuiWindowFlags flags = 
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("DEBUG", nullptr, flags);

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Dust Count: %d", s_dustCount);
        ImGui::Text("Time Speed: %.2f", s_timeSpeed);

        ImGui::End();
    }

    void Manager::EndFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Manager::Shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
#endif
