#include <glad/glad.h>

#include "simulation/dustSim.h"
#include "engine/Time.h"

void dustSimulation::update(float frameDt)
{
    accumulator += frameDt;
    if (accumulator > 0.25f)
        accumulator = 0.25f;

    while (accumulator >= fixedDt)
    {
        stepPhysics();
        accumulator -= fixedDt;
    }
}

void dustSimulation::stepPhysics()
{
    const GLuint count = dustPoints->getDustCount();
    const GLuint groups = (count + 255) / 256;

    gravity_shader->bind();
    glUniform1f(glGetUniformLocation(gravity_shader->getProgram(), "u_Softening"), 0.01);
    glUniform1f(glGetUniformLocation(gravity_shader->getProgram(), "u_MaxAccel"), 1000.0);
    glDispatchCompute(groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    dust_integrate_shader->bind();
    glUniform1f(glGetUniformLocation(dust_integrate_shader->getProgram(), "u_DeltaTime"), Time::control(dt));
    glDispatchCompute(groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}