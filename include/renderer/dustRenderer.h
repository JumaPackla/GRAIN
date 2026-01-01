#pragma once

#include <vector>
#include <glad/glad.h>

#include "particles/dustBody.h"

class dustRenderer
{
public:
    dustRenderer(const std::vector<dustBody>& initialBodies);

    dustRenderer(const dustRenderer&) = delete;
    dustRenderer& operator=(const dustRenderer&) = delete;

    dustRenderer(dustRenderer&& other) noexcept;
    dustRenderer& operator=(dustRenderer&& other) noexcept;

    ~dustRenderer();

    void draw();

    //GLuint getSSBO() const { return SSBO; }
    size_t getDustCount() const { return dustCount; }

private:
    GLuint VAO = 0;
    GLuint SSBO = 0;

    size_t dustCount = 0;
};
