#pragma once
#include <glad/glad.h>
#include <iostream>
#include <vector>

class debugBuffer {
public:
    debugBuffer(GLsizeiptr bufferSize) : size(static_cast<GLuint>(bufferSize / sizeof(float))) {
        glGenBuffers(1, &debugSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, nullptr, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, debugSSBO);
    }

    ~debugBuffer() {
        if (debugSSBO != 0) {
            glDeleteBuffers(1, &debugSSBO);
        }
    }

    GLuint getDebugSSBO() const { return debugSSBO; }

    void printFloat(const std::string& name = "debugBuffer") const {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
        float* data = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        if (!data) return;

        std::cout << "--- " << name << " ---\n";
        for (GLuint i = 0; i < size; ++i) {
            std::cout << "Val[" << i << "] = " 
                << data[i] << "\n";
        }
        std::cout << "----------------\n";

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }

    void printVec4(const std::string& name = "debugBuffer") const {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, debugSSBO);
        struct Vec4 { float x, y, z, w; };
        Vec4* data = (Vec4*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        if (!data) return;

        std::cout << "--- " << name << " ---\n";
        for (GLuint i = 0; i < size/4; ++i) {
            std::cout << "Val[" << i << "] = ("
                << data[i].x << ", "
                << data[i].y << ", "
                << data[i].z << ", "
                << data[i].w << ")\n";
        }
        std::cout << "----------------\n";

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }

private:
    GLuint debugSSBO = 0;
    GLuint size = 0;
};