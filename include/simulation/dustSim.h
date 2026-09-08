#pragma once

#include <cstdint>
#include "engine/Shader.h"

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
    void init();
    void update(float frameDt);

<<<<<<< HEAD
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

=======
    void setShader(std::unique_ptr<Shader> gravity, std::unique_ptr<Shader> integrate) {
        gravity_shader = std::move(gravity);
        dust_integrate_shader = std::move(integrate);
    }
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)
    void setSoftening(float s) { softening = s; }
    void setMaxAccel(float a) { maxAccel = a; }

private:
<<<<<<< HEAD
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
=======
    void stepPhysics();
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)

    float accumulator = 0.0f;
    float fixedDt = 1.0f / 120.0f;

    float softening = 0.01f;
    float maxAccel = 1000.0f;

    std::unique_ptr<Shader> gravity_shader;
<<<<<<< HEAD
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
=======
    std::unique_ptr<Shader> dust_integrate_shader;
};
>>>>>>> parent of c857c17 (uh leapforg works ig but slow)
