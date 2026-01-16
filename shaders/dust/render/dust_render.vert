#version 450 core

struct dustBody
{
    vec4 position;
    vec4 velocity;
    float radius;
    float mass;
    float _pad0;
    float _pad1;
};

layout(std430, binding = 0) buffer DustBuffer
{
    dustBody particles[];
};

uniform mat4 u_View;
uniform mat4 u_ViewProjection;

void main()
{
    vec3 pos = particles[gl_VertexID].position.xyz;
    vec4 viewPos = u_View * vec4(pos, 1.0);

    float depth = -viewPos.z;
    if (depth <= 0.0) {
        gl_PointSize = 0.0;
    } else {
        float radius = particles[gl_VertexID].radius;

        float scale = 500.0;

        gl_PointSize = ((radius * scale) / depth) * 50.0f;
        gl_PointSize = clamp(gl_PointSize, 1.0, 128.0);
    }

    gl_Position = u_ViewProjection * vec4(pos, 1.0);
}