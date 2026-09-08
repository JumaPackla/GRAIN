#include "simulation/dustSim.h"

void dustSimulation::init(size_t particleCount, const void* initialData)
{
    dustCount = particleCount;

    glGenBuffers(1, &simSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, simSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(dustBody), initialData, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, simSSBO);

    glGenBuffers(1, &mortonSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mortonSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, mortonSSBO);

    glGenBuffers(1, &mortonTempSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mortonTempSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, mortonTempSSBO);

    nodeCapacity = 2 * dustCount - 1;
    glGenBuffers(1, &nodeSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, nodeCapacity * sizeof(OctreeNode), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, nodeSSBO);

    glGenBuffers(1, &histSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, histSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 16 * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, histSSBO);

    glGenBuffers(1, &prefixSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, prefixSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 16 * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, prefixSSBO);

    glGenBuffers(1, &indexSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, indexSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, indexSSBO);

    glGenBuffers(1, &indexTempSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, indexTempSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, indexTempSSBO);

    std::vector<GLuint> indices(dustCount);
    for (size_t i = 0; i < dustCount; ++i)
        indices[i] = static_cast<GLuint>(i);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, indexSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, dustCount * sizeof(GLuint), indices.data());

    stepGravity();

    initLeapfrog(fixedDt);
    leapfrogInitialized = true;
}

void dustSimulation::setShader(
    std::unique_ptr<Shader> gravity,
    std::unique_ptr<Shader> leapfrog_init,
    std::unique_ptr<Shader> leapfrog_step,
    std::unique_ptr<Shader> morton,
    std::unique_ptr<Shader> radix_histogram,
    std::unique_ptr<Shader> radix_scan,
    std::unique_ptr<Shader> radix_scatter,
    std::unique_ptr<Shader> lvbh_init_leaves,
    std::unique_ptr<Shader> lvbh_build,
    std::unique_ptr<Shader> lvbh_mass)
{
    gravity_shader = std::move(gravity);
    leapfrog_init_shader = std::move(leapfrog_init);
    leapfrog_step_shader = std::move(leapfrog_step);

    morton_shader = std::move(morton);

    radix_histogram_shader = std::move(radix_histogram);
    radix_scan_shader = std::move(radix_scan);
    radix_scatter_shader = std::move(radix_scatter);

    lvbh_init_leaves_shader = std::move(lvbh_init_leaves);
    lvbh_build_shader = std::move(lvbh_build);
    lvbh_mass_shader = std::move(lvbh_mass);
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

void dustSimulation::stepPhysics(float dt)
{
    stepGravity();

    GLuint count = static_cast<GLuint>(dustCount);
    constexpr GLuint LOCAL_SIZE = 256;
    GLuint groups = (count + LOCAL_SIZE - 1) / LOCAL_SIZE;

    leapfrog_step_shader->bind();
    leapfrog_step_shader->setUniform("u_DeltaTime", dt);
    leapfrog_step_shader->setUniform("u_ParticleCount", count);

    glDispatchCompute(groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void dustSimulation::stepGravity()
{
    GLuint count = static_cast<GLuint>(dustCount);

    {
        GLuint groups = (count + 255) / 256;

        morton_shader->bind();
        morton_shader->setUniform("u_ParticleCount", count);

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    for (GLuint bit = 0; bit < 32; bit += 4)
    {
        GLuint zero[16] = {};
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, histSSBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(zero), zero);

        radix_histogram_shader->bind();
        radix_histogram_shader->setUniform("u_ParticleCount", count);
        radix_histogram_shader->setUniform("u_BitOffset", bit);
        glDispatchCompute((count + 255) / 256, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        radix_scan_shader->bind();
        glDispatchCompute(1, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        radix_scatter_shader->bind();
        radix_scatter_shader->setUniform("u_ParticleCount", count);
        radix_scatter_shader->setUniform("u_BitOffset", bit);
        glDispatchCompute((count + 255) / 256, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        std::swap(mortonSSBO, mortonTempSSBO);
        std::swap(indexSSBO, indexTempSSBO);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, mortonSSBO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, mortonTempSSBO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, indexSSBO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, indexTempSSBO);

    }

    {
        GLuint groups = (count + 255) / 256;

        lbvh_init_leaves_shader->bind();
        lbvh_init_leaves_shader->setUniform("u_ParticleCount", count);

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    {
        GLuint groups = ((count - 1) + 255) / 256;

        lbvh_build_shader->bind();
        lbvh_build_shader->setUniform("u_ParticleCount", count);

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    {
        GLuint groups = ((count - 1) + 255) / 256;

        lbvh_mass_shader->bind();
        lbvh_mass_shader->setUniform("u_ParticleCount", count);

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }

    {
        GLuint groups = (count + 255) / 256;

        gravity_shader->bind();
        gravity_shader->setUniform("u_ParticleCount", count);
        gravity_shader->setUniform("u_NodeCount", (GLuint)nodeCapacity);
        gravity_shader->setUniform("u_Theta", 0.6f);
        gravity_shader->setUniform("u_Softening", softening);

        glDispatchCompute(groups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
}