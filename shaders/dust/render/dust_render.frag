#version 450 core
out vec4 FragColor;

void main()
{
    vec2 uv = gl_PointCoord * 2.0 - 1.0;
    if (dot(uv, uv) > 1.0) discard;

    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
