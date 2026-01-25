#include "renderer/dustRenderer.h"
#include <algorithm>
#include <cstring>

dustRenderer::dustRenderer(const std::vector<dustBody>& initialBodies)
{
    dustCount = initialBodies.size();
    chunkCount = (dustCount + 255) / 256;

    glGenVertexArrays(1, &VAO);

    // Binding 0: Simulation data
    glGenBuffers(1, &simSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, simSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(dustBody), initialBodies.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, simSSBO);

    // Binding 1: Render positions
    glGenBuffers(1, &renderSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, renderSSBO);

    // Binding 2: Visible particles output
    glGenBuffers(1, &visibleSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, visibleSSBO);

    // Compute actual chunk bounds from particle positions
    std::vector<DustChunk> chunks(chunkCount);
    const uint32_t particlesPerChunk = 256;

    for (size_t i = 0; i < chunkCount; ++i)
    {
        uint32_t start = i * particlesPerChunk;
        uint32_t end = std::min(start + particlesPerChunk, static_cast<uint32_t>(dustCount));
        uint32_t count = end - start;

        // Compute AABB for this chunk
        glm::vec3 minBound(FLT_MAX);
        glm::vec3 maxBound(-FLT_MAX);

        for (uint32_t j = start; j < end; ++j)
        {
            glm::vec3 pos = glm::vec3(initialBodies[j].position);
            minBound = glm::min(minBound, pos);
            maxBound = glm::max(maxBound, pos);
        }

        glm::vec3 center = (minBound + maxBound) * 0.5f;
        float radius = glm::length(maxBound - center);

        chunks[i].center = center;
        chunks[i].radius = radius;
        chunks[i].start = start;
        chunks[i].count = count;
    }

    // Binding 3: Chunk metadata
    glGenBuffers(1, &chunkSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, chunkSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, chunks.size() * sizeof(DustChunk), chunks.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, chunkSSBO);

    // Binding 4: Visible particle count
    glGenBuffers(1, &visibleCountSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleCountSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, visibleCountSSBO);

    // Binding 5: Per-chunk visibility flags
    glGenBuffers(1, &chunkVisibleSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, chunkVisibleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, chunkCount * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, chunkVisibleSSBO);

    // Binding 6: Indirect draw command
    DrawArraysIndirectCommand cmd{};
    cmd.count = 0;
    cmd.instanceCount = 1;
    cmd.first = 0;
    cmd.baseInstance = 0;

    glGenBuffers(1, &indirectBuffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawArraysIndirectCommand), &cmd, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, indirectBuffer);
}

dustRenderer::~dustRenderer()
{
    destroy();
}

void dustRenderer::destroy()
{
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (simSSBO) glDeleteBuffers(1, &simSSBO);
    if (renderSSBO) glDeleteBuffers(1, &renderSSBO);
    if (visibleSSBO) glDeleteBuffers(1, &visibleSSBO);
    if (visibleCountSSBO) glDeleteBuffers(1, &visibleCountSSBO);
    if (chunkSSBO) glDeleteBuffers(1, &chunkSSBO);
    if (chunkVisibleSSBO) glDeleteBuffers(1, &chunkVisibleSSBO);
    if (indirectBuffer) glDeleteBuffers(1, &indirectBuffer);
    VAO = 0;
}

dustRenderer::dustRenderer(dustRenderer&& other) noexcept
{
    *this = std::move(other);
}

dustRenderer& dustRenderer::operator=(dustRenderer&& other) noexcept
{
    if (this != &other)
    {
        destroy();
        VAO = other.VAO;
        simSSBO = other.simSSBO;
        renderSSBO = other.renderSSBO;
        visibleSSBO = other.visibleSSBO;
        visibleCountSSBO = other.visibleCountSSBO;
        chunkSSBO = other.chunkSSBO;
        chunkVisibleSSBO = other.chunkVisibleSSBO;
        indirectBuffer = other.indirectBuffer;
        dustCount = other.dustCount;
        chunkCount = other.chunkCount;

        other.VAO = 0;
        other.simSSBO = 0;
        other.renderSSBO = 0;
        other.visibleSSBO = 0;
        other.visibleCountSSBO = 0;
        other.chunkSSBO = 0;
        other.chunkVisibleSSBO = 0;
        other.indirectBuffer = 0;
        other.dustCount = 0;
        other.chunkCount = 0;
    }
    return *this;
}

void dustRenderer::drawParticles()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
    glDrawArraysIndirect(GL_POINTS, nullptr);
    glBindVertexArray(0);
}

void dustRenderer::drawChunks()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(chunkCount));
    glBindVertexArray(0);
}