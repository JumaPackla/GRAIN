#pragma once

#include <vector>
#include <glad/glad.h>

#include "particles/dustBody.h"


struct DrawArraysIndirectCommand {
    GLuint count;
    GLuint instanceCount;
    GLuint first;
    GLuint baseInstance;
};

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

    size_t getDustCount() const { return dustCount; };

    GLuint getSimSSBO() const { return simSSBO; };
    GLuint getRenderSSBO() const { return renderSSBO; };
    GLuint getVisibleSSBO() const { return visibleSSBO; };
    GLuint getTempCountsSSBO() const { return tempCountsSSBO; };
    GLuint getIndirectBuffer() const { return indirectBuffer; };

private:
    GLuint VAO = 0;
    GLuint simSSBO = 0;
    GLuint renderSSBO = 0;
    GLuint visibleSSBO = 0;
    GLuint tempCountsSSBO = 0;
    GLuint indirectBuffer = 0;

    size_t dustCount = 0;
};