#pragma once

#include <cstdint>
#include "engine/Shader.h"

class dustSimulation {
public:
    void init();
    void update(float frameDt);

    void setShader(std::unique_ptr<Shader> gravity, std::unique_ptr<Shader> integrate) {
        gravity_shader = std::move(gravity);
        dust_integrate_shader = std::move(integrate);
    }
    void setSoftening(float s) { softening = s; }
    void setMaxAccel(float a) { maxAccel = a; }

private:
    void stepPhysics();

    float accumulator = 0.0f;
    float fixedDt = 1.0f / 120.0f;

    float softening = 0.01f;
    float maxAccel = 1000.0f;

    std::unique_ptr<Shader> gravity_shader;
    std::unique_ptr<Shader> dust_integrate_shader;
};