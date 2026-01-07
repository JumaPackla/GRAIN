#version 450 core
out vec4 FragColor;

void main()
{
    vec2 c = gl_PointCoord - vec2(0.5);
    if (length(c) > 0.5) discard;

    FragColor = vec4(1.0);
}