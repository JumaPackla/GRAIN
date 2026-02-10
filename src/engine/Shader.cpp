#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <algorithm>

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

void Shader::checkCompileErrors(GLuint shader, const std::string& type, const std::string& path, const std::string& source)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        std::cerr << "[ERROR] " << type << " shader compilation failed: " << path << "\n" << log.data() << "\n";

        std::stringstream srcStream(source);
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(srcStream, line)) {
            lines.push_back(line);
        }

        std::stringstream logStream(log.data());
        std::string logLine;
        while (std::getline(logStream, logLine)) {
            size_t start = logLine.find('(');
            size_t end = logLine.find(')');
            if (start != std::string::npos && end != std::string::npos && end > start + 1) {
                int lineNum = std::stoi(logLine.substr(start + 1, end - start - 1));
                int from = std::max(0, lineNum - 3);
                int to = std::min((int)lines.size(), lineNum + 2);
                for (int i = from; i < to; i++) {
                    std::cerr << (i + 1) << ": " << lines[i] << "\n";
                }
            }
        }
    }
}

void Shader::checkLinkErrors(GLuint program, const std::string& type)
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        std::cerr << "[ERROR] " << type << " program linking failed\n" << log.data() << "\n";
    }
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
    checkCompileErrors(vertexShader, "Vertex", vertexPath, vertexCode);

    const char* fSrc = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fSrc, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "Fragment", fragmentPath, fragmentCode);

    compileAndLink(vertexShader, fragmentShader);

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
    checkCompileErrors(computeShader, "Compute", computePath, code);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, computeShader);
    glLinkProgram(shader_program);
    checkLinkErrors(shader_program, "Compute");

    glDeleteShader(computeShader);
}

void Shader::compileAndLink(GLuint vertexShader, GLuint fragmentShader)
{
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);
    checkLinkErrors(shader_program, "Vertex/Fragment");
}

void Shader::setUniform(const char* name, float v) const
{
    GLint loc = glGetUniformLocation(shader_program, name);
    if (loc != -1)
        glUniform1f(loc, v);
}

void Shader::setUniform(const char* name, unsigned int v) const
{
    GLint loc = glGetUniformLocation(shader_program, name);
    if (loc != -1)
        glUniform1ui(loc, v);
}

void Shader::setUniform(const char* name, int v) const
{
    GLint loc = glGetUniformLocation(shader_program, name);
    if (loc != -1)
        glUniform1i(loc, v);
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
