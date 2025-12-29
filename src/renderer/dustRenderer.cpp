#include "renderer/dustRenderer.h"

dustRenderer::dustRenderer(const std::vector<dustBody>& bodies) : dustBodies(bodies)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (!dustBodies.empty()) {
        glBufferData(GL_ARRAY_BUFFER, dustBodies.size() * sizeof(dustBody), dustBodies.data(), GL_DYNAMIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(dustBody), (void*)offsetof(dustBody, position));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

dustRenderer::dustRenderer(dustRenderer&& other) noexcept : VAO(other.VAO), VBO(other.VBO), dustBodies(std::move(other.dustBodies))
{
    other.VAO = 0;
    other.VBO = 0;
}

dustRenderer& dustRenderer::operator=(dustRenderer&& other) noexcept
{
    if (this != &other) {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);

        VAO = other.VAO;
        VBO = other.VBO;
        dustBodies = std::move(other.dustBodies);

        other.VAO = 0;
        other.VBO = 0;
    }
    return *this;
}

dustRenderer::~dustRenderer()
{
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

void dustRenderer::draw()
{

}