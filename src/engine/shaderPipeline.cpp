#include <iostream>
#include <fstream>

#include "engine/shaderPipeline.h"

std::string readFile(std::string& filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		std::cout << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	};
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	};
	fileStream.close();
	return content;
};

shaderPipeline::shaderPipeline(std::string vertex_shader_location, std::string fragment_shader_location) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vertexShaderCode = readFile(vertex_shader_location);
	std::string fragmentShaderCode = readFile(fragment_shader_location);

	const char* vertexSourcePtr = vertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &vertexSourcePtr, NULL);
	glCompileShader(vertexShader);
	const char* fragmentSourcePtr = fragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSourcePtr, NULL);
	glCompileShader(fragmentShader);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertexShader);
	glAttachShader(shader_program, fragmentShader);
	glLinkProgram(shader_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (glGetError() != GL_NO_ERROR) {
		std::cout << "Error creating shader program from " << vertex_shader_location << " and " << fragment_shader_location << std::endl;
	};
	int success;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetProgramInfoLog(shader_program, 512, nullptr, log);
		std::cerr << "Shader program link error:\n" << log << std::endl;
	}
}

shaderPipeline::~shaderPipeline() {
	glDeleteProgram(shader_program);
}

int shaderPipeline::bind() {
	glUseProgram(shader_program);
	return 0;
}