#version 330 core

// output
out vec4 FragColor;

// uniform
uniform float time;
uniform vec2 resolution;

float sdCircle(vec2 p, float r)
{
    return length(p) - r;
}

void main()
{
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);

    float d = sdCircle(p, 0.5);

    vec3 c = vec3(pow(1.0-d, 5.0));

    FragColor = vec4(vec3(c), 1.0);
}