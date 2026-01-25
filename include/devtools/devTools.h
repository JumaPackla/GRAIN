#pragma once
#include <memory>

struct GLFWwindow;

namespace devTools {

    class Manager {
    public:
        static void Init(GLFWwindow* window);
        static void SetDustCount(int dustCount);
        static void SetTimeSpeed(float timeSpeed);
        static void BeginFrame();
        static void EndFrame();
        static void Shutdown();
    private:
        static int s_dustCount;
        static float s_timeSpeed;
    };

}
