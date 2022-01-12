#version 330 core

// output
out vec4 FragColor;

// uniform
uniform float time;
uniform vec2 resolution;

float sdBox(vec2 p, vec2 b)
{
    vec2 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

void main()
{
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);

    float d = sdBox(p, vec2(0.2, 0.4));

    vec3 c = vec3(pow(1.0-d, 5.0));

    FragColor = vec4(vec3(c), 1.0);
}