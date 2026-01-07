#version 450 core
out vec4 FragColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec2 c = gl_PointCoord - vec2(0.5);
    if (length(c) > 0.5) discard;

    vec2 uv = gl_FragCoord.xy; // pixel coordinates
    vec3 color = vec3(rand(uv + 0.1), rand(uv + 0.2), rand(uv + 0.3));

    FragColor = vec4(color, 1.0);
}