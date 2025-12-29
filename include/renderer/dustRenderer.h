#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "particles/dustBody.h"

struct dustBody;

class dustRenderer
{
public:
    dustRenderer(const std::vector<dustBody>& bodies);
    
    dustRenderer(const dustRenderer&) = delete;
    dustRenderer& operator=(const dustRenderer&) = delete;

    dustRenderer(dustRenderer&& other) noexcept;
    dustRenderer& operator=(dustRenderer&& other) noexcept;

    ~dustRenderer();

    void draw();

private:
    GLuint VAO = 0;
    GLuint VBO = 0;

    std::vector<dustBody> dustBodies;
};