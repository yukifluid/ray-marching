#version 330 core

out vec4 FragColor;

uniform float time;
uniform vec2 resolution;

const float sphereRadius = 1.0;
const vec3 lightDir = vec3(-0.577, 0.577, 0.577);

float sdf(vec3 p)
{
    return length(p) - sphereRadius;
}

vec3 getNormal(vec3 p)
{
    float d = 0.0001;
   return normalize(vec3(
        sdf(p + vec3(  d, 0.0, 0.0)) - sdf(p + vec3( -d, 0.0, 0.0)),
        sdf(p + vec3(0.0,   d, 0.0)) - sdf(p + vec3(0.0,  -d, 0.0)),
        sdf(p + vec3(0.0, 0.0,   d)) - sdf(p + vec3(0.0, 0.0,  -d))
    )); 
}

void main()
{
   // fragment position
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    
    // camera
    vec3 cPos = vec3(0.0,  0.0,  2.0);
    vec3 cDir = vec3(0.0,  0.0, -1.0);
    vec3 cUp  = vec3(0.0,  1.0,  0.0);
    vec3 cSide = cross(cDir, cUp);
    float targetDepth = 1.0;
    
    // ray
    vec3 ray = normalize(cSide * p.x + cUp * p.y + cDir * targetDepth);

    // marching loop
    float distance = 0.0;
    float rLen = 0.0;
    vec3 rPos = cPos;
    for(int i = 0; i < 16; i++)
    {
        distance = sdf(rPos);
        rLen += distance;
        rPos = cPos + ray * rLen;
    }

    if(abs(distance) < 0.001)
    {
        vec3 normal = getNormal(rPos);
        float diff = clamp(dot(lightDir, normal), 0.1, 1.0);
        FragColor = vec4(vec3(diff), 1.0);
    }
    else
    {
        FragColor = vec4(vec3(0.0), 1.0);
    }
}