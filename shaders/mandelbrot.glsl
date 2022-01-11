#version 330 core

out vec4 FragColor;

uniform float time;
uniform vec2 resolution;

vec3 hsv(float h, float s, float v)
{
    vec4 t = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(vec3(h) + t.xyz) * 6.0 - vec3(t.w));
    return v * mix(vec3(t.x), clamp(p - vec3(t.x), 0.0, 1.0), s);
}

void main()
{
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);

    int j = 0;
    vec2 x = p + vec2(-0.5, 0.0);
    vec2 z = vec2(0.0, 0.0);

    for(int i = 0; i < 350; i++)
    {
        j++;
        if(length(z) > 2.0) break;
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + x;
    }

    float t = float(j) / 360.0;

    FragColor = vec4(vec3(t), 1.0);
}