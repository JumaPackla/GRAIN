#version 450 core

in vec3  vWorldPos;
in float vRadius;

out vec4 FragColor;

uniform vec3  u_CameraPos;
uniform float u_LodNear;
uniform float u_LodFar;

void main()
{
    vec2 uv = gl_PointCoord * 2.0 - 1.0;
    float r = dot(uv, uv);
    if (r > 1.0) discard;

    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
