#version 450 core

struct DustChunk {
    vec3 center;
    float radius;
    uint start;
    uint count;
    uint _pad0;
    uint _pad1;
};

layout(std430, binding = 3) buffer ChunkBuffer {
    DustChunk chunks[];
};

uniform mat4 u_ViewProjection;
uniform vec3 u_CameraPos;
uniform float u_PointScale;

out vec3 vWorldPos;
out float vRadius;

void main()
{
    DustChunk c = chunks[gl_VertexID];
    vWorldPos = c.center;
    vRadius = c.radius;
    
    float dist = length(c.center - u_CameraPos);
    gl_PointSize = max((c.radius / dist) * u_PointScale, 1.0);
    gl_Position = u_ViewProjection * vec4(c.center, 1.0);
}