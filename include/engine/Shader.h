#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
public:
	Shader(std::string vertex_shader_location, std::string fragment_shader_location);
	~Shader();

	int bind();
	GLuint getProgram() const { return shader_program; }

private:
	GLuint shader_program;

};