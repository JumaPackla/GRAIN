#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& computePath);
    ~Shader();

    void bind() const;

    static std::string getShaderPath(const std::string& relativePath);
    static std::string readFile(const std::string& filePath);

    void setUniform(const char* name, float v) const;
    void setUniform(const char* name, unsigned int v) const;
    void setUniform(const char* name, int v) const;

    GLuint getProgram() const { return shader_program; }

private:
    GLuint shader_program = 0;

    void compileAndLink(GLuint vertexShader, GLuint fragmentShader);

    void checkCompileErrors(GLuint shader, const std::string& type, const std::string& path, const std::string& source);
    void checkLinkErrors(GLuint program, const std::string& type);
};