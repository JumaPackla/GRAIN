#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "engine/Shader.h"

std::string Shader::getShaderPath(const std::string& relativePath)
{
#if defined(SHADER_SOURCE_DIR) && defined(_DEBUG)
    return std::string(SHADER_SOURCE_DIR) + "/" + relativePath;
#elif defined(SHADER_SOURCE_DIR)
    return std::filesystem::current_path().string() + "/shaders/" + relativePath;
#else
    std::cerr << "Warning: SHADER_SOURCE_DIR not defined. Using current directory.\n";
    return std::filesystem::current_path().string() + "/" + relativePath;
#endif
}

std::string Shader::readFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not read file: " << filePath << std::endl;
        throw std::runtime_error("Failed to open shader file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexCode = readFile(getShaderPath(vertexPath));
    std::string fragmentCode = readFile(getShaderPath(fragmentPath));

    const char* vSrc = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vSrc, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, log);
        std::cerr << "Vertex shader compile error:\n" << log << "\n";
    }

    const char* fSrc = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
        std::cerr << "Fragment shader compile error:\n" << log << "\n";
    }

    this->compileAndLink(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string& computePath)
{
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    std::string code = readFile(getShaderPath(computePath));
    const char* src = code.c_str();

    glShaderSource(computeShader, 1, &src, nullptr);
    glCompileShader(computeShader);

    GLint success;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(computeShader, 512, nullptr, log);
        std::cerr << "Compute shader compile error:\n" << log << "\n";
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, computeShader);
    glLinkProgram(shader_program);

    glDeleteShader(computeShader);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(shader_program, 512, nullptr, log);
        std::cerr << "Compute shader program link error:\n" << log << "\n";
    }
}

void Shader::compileAndLink(GLuint vertexShader, GLuint fragmentShader)
{
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(shader_program, 512, nullptr, log);
        std::cerr << "Shader program link error:\n" << log << "\n";
    }
}

Shader::~Shader()
{
    if (shader_program) {
        glDeleteProgram(shader_program);
    }
}

void Shader::bind() const
{
	glUseProgram(shader_program);
}