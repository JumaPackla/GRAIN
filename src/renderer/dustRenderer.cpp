#include "renderer/dustRenderer.h"

dustRenderer::dustRenderer(const std::vector<dustBody>& initialBodies)
{
    dustCount = initialBodies.size();

    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dustCount * sizeof(dustBody), initialBodies.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

    glGenVertexArrays(1, &VAO);
}

dustRenderer::~dustRenderer()
{
    if (SSBO) glDeleteBuffers(1, &SSBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

dustRenderer::dustRenderer(dustRenderer&& other) noexcept
{
    VAO = other.VAO;
    SSBO = other.SSBO;
    dustCount = other.dustCount;

    other.VAO = 0;
    other.SSBO = 0;
    other.dustCount = 0;
}

dustRenderer& dustRenderer::operator=(dustRenderer&& other) noexcept
{
    if (this != &other)
    {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (SSBO) glDeleteBuffers(1, &SSBO);

        VAO = other.VAO;
        SSBO = other.SSBO;
        dustCount = other.dustCount;

        other.VAO = 0;
        other.SSBO = 0;
        other.dustCount = 0;
    }
    return *this;
}

void dustRenderer::draw()
{
    glBindVertexArray(VAO);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(dustCount));
    glEnable(GL_DEPTH_TEST);
}