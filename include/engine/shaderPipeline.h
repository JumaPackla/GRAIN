#pragma once

#include <string>
#include <glad/glad.h>

class shaderPipeline
{
public:
	shaderPipeline(std::string vertex_shader_location, std::string fragment_shader_location);
	~shaderPipeline();

	int bind();
	GLuint getProgram() const { return shader_program; }

private:
	GLuint shader_program;

};