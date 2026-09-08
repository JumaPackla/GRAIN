#pragma once

#include <cstddef>
#include <glad/glad.h>

#include "simulation/dustSim.h"

struct DrawArraysIndirectCommand {
    GLuint count;
    GLuint instanceCount;
    GLuint first;
    GLuint baseInstance;
};

class dustRenderer {
public:
    explicit dustRenderer(dustSimulation& sim);

    dustRenderer(const dustRenderer&) = delete;
    dustRenderer& operator=(const dustRenderer&) = delete;

    dustRenderer(dustRenderer&& other) noexcept;
    dustRenderer& operator=(dustRenderer&& other) noexcept;

    ~dustRenderer();

    void draw();

    GLuint getRenderSSBO() const { return renderSSBO; }
    GLuint getVisibleSSBO() const { return visibleSSBO; }
    GLuint getTempCountsSSBO() const { return tempCountsSSBO; }
    GLuint getIndirectBuffer() const { return indirectBuffer; }

    size_t getDustCount() const { return dustCount; }

private:
    dustSimulation& simulation;

    GLuint VAO = 0;

    GLuint renderSSBO = 0;
    GLuint visibleSSBO = 0;
    GLuint tempCountsSSBO = 0;
    GLuint indirectBuffer = 0;

    size_t dustCount = 0;
};
