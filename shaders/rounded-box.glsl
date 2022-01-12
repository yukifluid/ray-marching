#version 330 core

// output
out vec4 FragColor;

// uniform
uniform float time;
uniform vec2 resolution;

float sdRoundedBox(vec2 p, vec2 b, vec4 r)
{
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x  = (p.y>0.0)?r.x  : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}

void main()
{
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);

    float d = sdRoundedBox(p, vec2(0.4, 0.2), vec4(0.1, 0.1, 0.3, 0.3));

    vec3 c = vec3(pow(1.0-d, 5.0));

    FragColor = vec4(vec3(c), 1.0);
}