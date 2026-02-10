#include <glm/glm.hpp>

#include "renderer/dustRenderer.h"

dustRenderer::dustRenderer(dustSimulation& sim) : simulation(sim){
    dustCount = simulation.getDustCount();
    const GLuint groups = static_cast<GLuint>((dustCount + 255) / 256);

    glGenBuffers(1, &renderSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, renderSSBO);

    glGenBuffers(1, &visibleSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, visibleSSBO);

    glGenBuffers(1, &tempCountsSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempCountsSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, groups * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, tempCountsSSBO);

    DrawArraysIndirectCommand cmd{ 0, 1, 0, 0 };
    glGenBuffers(1, &indirectBuffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawArraysIndirectCommand), &cmd, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, indirectBuffer);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, renderSSBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

    glBindVertexArray(0);
}

dustRenderer::~dustRenderer()
{
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (renderSSBO) glDeleteBuffers(1, &renderSSBO);
    if (visibleSSBO) glDeleteBuffers(1, &visibleSSBO);
    if (tempCountsSSBO) glDeleteBuffers(1, &tempCountsSSBO);
    if (indirectBuffer) glDeleteBuffers(1, &indirectBuffer);
}

dustRenderer::dustRenderer(dustRenderer&& other) noexcept
    : simulation(other.simulation),
    VAO(other.VAO),
    renderSSBO(other.renderSSBO),
    visibleSSBO(other.visibleSSBO),
    tempCountsSSBO(other.tempCountsSSBO),
    indirectBuffer(other.indirectBuffer),
    dustCount(other.dustCount)
{
    other.VAO = 0;
    other.renderSSBO = 0;
    other.visibleSSBO = 0;
    other.tempCountsSSBO = 0;
    other.indirectBuffer = 0;
    other.dustCount = 0;
}

dustRenderer& dustRenderer::operator=(dustRenderer&& other) noexcept
{
    if (this != &other)
    {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (renderSSBO) glDeleteBuffers(1, &renderSSBO);
        if (visibleSSBO) glDeleteBuffers(1, &visibleSSBO);
        if (tempCountsSSBO) glDeleteBuffers(1, &tempCountsSSBO);
        if (indirectBuffer) glDeleteBuffers(1, &indirectBuffer);

        VAO = other.VAO;
        renderSSBO = other.renderSSBO;
        visibleSSBO = other.visibleSSBO;
        tempCountsSSBO = other.tempCountsSSBO;
        indirectBuffer = other.indirectBuffer;
        dustCount = other.dustCount;

        other.VAO = 0;
        other.renderSSBO = 0;
        other.visibleSSBO = 0;
        other.tempCountsSSBO = 0;
        other.indirectBuffer = 0;
        other.dustCount = 0;
    }
    return *this;
}

void dustRenderer::draw()
{
    glBindVertexArray(VAO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, simulation.getSimSSBO());
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(dustCount));

    // glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
    // glDrawArraysIndirect(GL_POINTS, nullptr);
}
