#pragma once
#include <GLFW/glfw3.h>
#include "engine/inputState.h"

struct GLFWInputAdapter
{
    static void pollInput(GLFWwindow* window, inputState& state)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        static bool prevE = false;
        bool eNow = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
        if (eNow && !prevE)
        {
            state.mouseCaptured = !state.mouseCaptured;
            if (state.mouseCaptured)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                int width, height;
                glfwGetWindowSize(window, &width, &height);
                double centerX = width / 2.0;
                double centerY = height / 2.0;
                glfwSetCursorPos(window, centerX, centerY);
                state.lastMouseX = centerX;
                state.lastMouseY = centerY;
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        prevE = eNow;

        static bool prevP = false;
        bool pNow = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
        state.togglePause = pNow && !prevP;
        prevP = pNow;

        if (state.mouseCaptured)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            state.mouseDeltaX = float(mouseX - state.lastMouseX);
            state.mouseDeltaY = float(state.lastMouseY - mouseY);
            state.lastMouseX = mouseX;
            state.lastMouseY = mouseY;

            state.moveForward = float(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) - float(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
            state.moveRight = float(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - float(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
            state.moveUp = float(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) - float(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
        }
        else
        {
            state.mouseDeltaX = 0.0f;
            state.mouseDeltaY = 0.0f;
            state.moveForward = 0.0f;
            state.moveRight = 0.0f;
            state.moveUp = 0.0f;
        }
    }

};
