#include "simulation/dustSim.h"

void dustSimulation::init(size_t particleCount, const void* initialData)
{
    dustCount = particleCount;

    glGenBuffers(1, &simSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, simSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(dustBody), initialData, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, simSSBO);

    {
        constexpr GLuint LOCAL_SIZE = 128;
        GLuint groups = (dustCount + LOCAL_SIZE - 1) / LOCAL_SIZE;

        gravity_shader->bind();
        gravity_shader->setUniform("u_ParticleCount", (GLuint)dustCount);
        gravity_shader->setUniform("u_Softening", softening);
        gravity_shader->setUniform("u_MaxAccel", maxAccel);

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    initLeapfrog(fixedDt);
    leapfrogInitialized = true;
}

void dustSimulation::setShader(std::unique_ptr<Shader> gravity, std::unique_ptr<Shader> leapfrog_init, std::unique_ptr<Shader> leapfrog_step)
{
    gravity_shader = std::move(gravity);
    leapfrog_init_shader = std::move(leapfrog_init);
    leapfrog_step_shader = std::move(leapfrog_step);
}

void dustSimulation::update(float dt)
{
    if (!leapfrogInitialized)
        return;

    accumulator += dt;
    if (accumulator > 0.25f)
        accumulator = 0.25f;

    while (accumulator >= fixedDt)
    {
        stepPhysics(fixedDt);
        accumulator -= fixedDt;
    }
}

void dustSimulation::stepPhysics(float dt)
{
    GLuint count = static_cast<GLuint>(dustCount);

    {
        constexpr GLuint LOCAL_SIZE = 128;
        GLuint groups = (count + LOCAL_SIZE - 1) / LOCAL_SIZE;

        gravity_shader->bind();
        gravity_shader->setUniform("u_ParticleCount", count);
        gravity_shader->setUniform("u_Softening", softening);
        gravity_shader->setUniform("u_MaxAccel", maxAccel);

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    {
        constexpr GLuint LOCAL_SIZE = 256;
        GLuint groups = (count + LOCAL_SIZE - 1) / LOCAL_SIZE;

        leapfrog_step_shader->bind();
        leapfrog_step_shader->setUniform("u_DeltaTime", dt);
        leapfrog_step_shader->setUniform("u_ParticleCount", count);

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
}

void dustSimulation::initLeapfrog(float dt)
{
    GLuint count = static_cast<GLuint>(dustCount);
    GLuint groups = (count + 255) / 256;

    leapfrog_init_shader->bind();
    leapfrog_init_shader->setUniform("u_DeltaTime", dt);
    leapfrog_init_shader->setUniform("u_ParticleCount", count);

    glDispatchCompute(groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
