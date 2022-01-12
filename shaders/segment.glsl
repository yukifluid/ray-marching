#version 330 core

// output
out vec4 FragColor;

// uniform
uniform float time;
uniform vec2 resolution;

float sdSegment( in vec2 p, in vec2 a, in vec2 b )
{
    vec2 pa = p-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length(pa-ba*h);
}

void main()
{
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);

    float d = sdSegment(p, vec2(0.5), vec2(-0.5));

    vec3 c = vec3(pow(1.0-d, 5.0));

    FragColor = vec4(vec3(c), 1.0);
}