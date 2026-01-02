#version 440 core

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

uniform mat4 u_ViewProjection;

void main()
{
    vec3 pos = particles[gl_VertexID].position.xyz;
    gl_Position = u_ViewProjection * vec4(pos, 1.0);

    //gl_PointSize = 1.0;
    gl_PointSize = particles[gl_VertexID].radius * 5.0;
}