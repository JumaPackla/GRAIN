#version 450 core

layout(std430, binding = 2) buffer VisibleBuffer {
    vec4 visiblePos[];
};

uniform mat4 u_ViewProjection;
uniform vec3 u_CameraPos;
uniform float u_PointScale;

void main()
{
    vec3 pos = visiblePos[gl_VertexID].xyz;

    float dist = length(pos - u_CameraPos);
    gl_PointSize = max(u_PointScale / dist, 1.0);

    gl_Position = u_ViewProjection * vec4(pos, 1.0);
}
