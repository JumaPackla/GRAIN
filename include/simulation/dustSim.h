#pragma once

#include <memory>
#include <cstddef>
#include <glad/glad.h>
#include "engine/Shader.h"
#include "particles/dustBody.h"

class dustSimulation {
public:
    void init(size_t particleCount, const void* initialData);
    void update(float frameDt);

    void setShader(std::unique_ptr<Shader> gravity, std::unique_ptr<Shader> leapfrog_init, std::unique_ptr<Shader> leapfrog_step);

    void setSoftening(float s) { softening = s; }
    void setMaxAccel(float a) { maxAccel = a; }

    size_t getDustCount() const { return dustCount; }
    GLuint getSimSSBO() const { return simSSBO; }

private:
    void stepPhysics(float dt);
    void initLeapfrog(float dt);

    GLuint simSSBO = 0;
    size_t dustCount = 0;

    float accumulator = 0.0f;
    float fixedDt = 1.0f / 120.0f;

    float softening = 0.01f;
    float maxAccel = 1000.0f;

    std::unique_ptr<Shader> gravity_shader;
    std::unique_ptr<Shader> leapfrog_init_shader;
    std::unique_ptr<Shader> leapfrog_step_shader;

    bool leapfrogInitialized = false;
};
