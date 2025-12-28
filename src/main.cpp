#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/shaderPipeline.h"
#include "engine/cameraClass.h"
#include "renderer/triangleMesh.h"
#include "renderer/sphereRenderer.h"
#include "renderer/sphereBody.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	if (!gladLoadGL()) {
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);

	std::vector<Vertex> vertices = {
		// Triangle 1
		{ { -0.8f,  0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.4f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ {  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },

		// Triangle 2
		{ { 0.2f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.6f, -0.5f, 0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 1.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
	};

	std::vector<GLuint> indices = {
		0,1,2,   // Triangle 1
		3,4,5   // Triangle 2
	};

	triangleMesh combinedMesh(vertices, indices);

	sphereBody sphereBody1;
	sphereRenderer sphereMesh1(sphereBody1, 50, 100, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

	shaderPipeline shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

	cameraClass camera;

	static double lastFrameTime = 0.0f;
	float cameraSpeed = 3.0f;

	double lastMouseX = 0.0, lastMouseY = 0.0;
	bool firstMouse = true;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		double currentFrameTime = glfwGetTime();
		double deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		double mouseDeltaX = 0.0, mouseDeltaY = 0.0;

		if (firstMouse) {
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			firstMouse = false;
		}
		mouseDeltaX = mouseX - lastMouseX;
		mouseDeltaY = lastMouseY - mouseY;
		lastMouseX = mouseX;
		lastMouseY = mouseY;

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		float aspectRatio = (float)width / (float)height;

		glm::vec3 moveDir(0.0f);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDir.z -= 1.0f;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDir.z += 1.0f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDir.x -= 1.0f;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDir.x += 1.0f;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) moveDir.y += 1.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) moveDir.y -= 1.0f;

		camera.move(moveDir * cameraSpeed * static_cast<float>(deltaTime));
		float sensitivity = 1.0f;
		camera.rotate(sensitivity * static_cast<float>(-mouseDeltaX), sensitivity * static_cast<float>(mouseDeltaY));

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
		glm::mat4 vp = projection * view;

		shader.bind();
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "u_ViewProjection"), 1, GL_FALSE, glm::value_ptr(vp));

		glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		combinedMesh.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

		sphereMesh1.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}