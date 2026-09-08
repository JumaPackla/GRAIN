#version 450 core

layout(std430, binding = 1) buffer DustRender {
    vec4 renderPos[];
};

uniform mat4 u_ViewProjection;

void main()
{
    vec3 pos = renderPos[gl_VertexID].xyz;
    gl_Position = u_ViewProjection * vec4(pos, 1.0);
    gl_PointSize = 1.0f;
}