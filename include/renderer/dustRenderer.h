#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "particles/dustBody.h"

struct DrawArraysIndirectCommand {
    GLuint count;
    GLuint instanceCount;
    GLuint first;
    GLuint baseInstance;
};

struct DustChunk {
    glm::vec3 center;
    float radius;
    uint32_t start;    // First particle index
    uint32_t count;    // Number of particles in chunk
    uint32_t _pad0;
    uint32_t _pad1;
};

class dustRenderer
{
public:
    explicit dustRenderer(const std::vector<dustBody>& initialBodies);
    ~dustRenderer();

    dustRenderer(const dustRenderer&) = delete;
    dustRenderer& operator=(const dustRenderer&) = delete;

    dustRenderer(dustRenderer&& other) noexcept;
    dustRenderer& operator=(dustRenderer&& other) noexcept;

    void drawParticles();
    void drawChunks();

    size_t getDustCount() const { return dustCount; }
    size_t getChunkCount() const { return chunkCount; }

    GLuint getSimSSBO() const { return simSSBO; }
    GLuint getRenderSSBO() const { return renderSSBO; }
    GLuint getVisibleSSBO() const { return visibleSSBO; }
    GLuint getVisibleCountSSBO() const { return visibleCountSSBO; }
    GLuint getChunkSSBO() const { return chunkSSBO; }
    GLuint getChunkVisibleSSBO() const { return chunkVisibleSSBO; }
    GLuint getIndirectBuffer() const { return indirectBuffer; }

private:
    void destroy();

    GLuint VAO = 0;

    GLuint simSSBO = 0;             // binding 0
    GLuint renderSSBO = 0;          // binding 1
    GLuint visibleSSBO = 0;         // binding 2
    GLuint chunkSSBO = 0;           // binding 3
    GLuint visibleCountSSBO = 0;    // binding 4
    GLuint chunkVisibleSSBO = 0;    // binding 5
    GLuint indirectBuffer = 0;      // binding 6

    size_t dustCount = 0;
    size_t chunkCount = 0;
};