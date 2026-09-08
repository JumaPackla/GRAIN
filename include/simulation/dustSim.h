#pragma once

#include <memory>
#include <cstddef>
#include <glad/glad.h>
#include "engine/Shader.h"
#include "particles/dustBody.h"

struct OctreeNode
{
    glm::vec4 centerOfMass;

    GLuint left;
    GLuint right;
    GLuint parent;
    GLuint isLeaf;
    GLuint particleIndex;
};

class dustSimulation {
public:
    void init(size_t particleCount, const void* initialData);
    void update(float frameDt);

    void setShader(
        std::unique_ptr<Shader> gravity,
        std::unique_ptr<Shader> leapfrog_init,
        std::unique_ptr<Shader> leapfrog_step,
        std::unique_ptr<Shader> morton,
        std::unique_ptr<Shader> radix_histogram,
        std::unique_ptr<Shader> radix_scan,
        std::unique_ptr<Shader> radix_scatter,
        std::unique_ptr<Shader> lvbh_init_leaves,
        std::unique_ptr<Shader> lvbh_build,
        std::unique_ptr<Shader> lvbh_mass);

    void setSoftening(float s) { softening = s; }
    void setMaxAccel(float a) { maxAccel = a; }

    size_t getDustCount() const { return dustCount; }
    GLuint getSimSSBO() const { return simSSBO; }

private:
    void stepPhysics(float dt);
    void initLeapfrog(float dt);
    void stepGravity();

    GLuint simSSBO = 0; // 1
    GLuint mortonSSBO = 0; // 6
    GLuint mortonTempSSBO = 0; // 7
    GLuint nodeSSBO = 0; // 8
    GLuint histSSBO = 0; // 9
    GLuint prefixSSBO = 0; // 10
    GLuint indexSSBO = 0; // 11
    GLuint indexTempSSBO = 0; // 12

    size_t nodeCapacity = 0;

    size_t dustCount = 0;

    float accumulator = 0.0f;
    float fixedDt = 1.0f / 120.0f;

    float softening = 0.01f;
    float maxAccel = 1000.0f;

    std::unique_ptr<Shader> gravity_shader;
    std::unique_ptr<Shader> leapfrog_init_shader;
    std::unique_ptr<Shader> leapfrog_step_shader;

    std::unique_ptr<Shader> morton_shader;

    std::unique_ptr<Shader> radix_histogram_shader;
    std::unique_ptr<Shader> radix_scan_shader;
    std::unique_ptr<Shader> radix_scatter_shader;

    std::unique_ptr<Shader> lvbh_init_leaves_shader;
    std::unique_ptr<Shader> lvbh_build_shader;
    std::unique_ptr<Shader> lvbh_mass_shader;

    bool leapfrogInitialized = false;
};
