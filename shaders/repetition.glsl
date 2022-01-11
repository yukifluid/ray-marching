#version 330 core

out vec4 FragColor;

uniform float time;
uniform vec2 resolution;

const float PI = 3.14159265;
const float angle = 60.0;
const float fov = angle * PI / 360.0;
const vec3 cPos = vec3(0.0,  0.0,  3.0);
const vec3 lightDir = vec3(-0.577, 0.577, 0.577);

const float sphereRadius = 1.0;

vec3 trans(vec3 p)
{
    return mod(p, 4.0) - 2.0;
}

float distanceFunc(vec3 p)
{
    return length(trans(p)) - sphereRadius;
}

vec3 getNormal(vec3 p)
{
    float d = 0.0001;
    return normalize(vec3(
        distanceFunc(p + vec3(  d, 0.0, 0.0)) - distanceFunc(p + vec3( -d, 0.0, 0.0)),
        distanceFunc(p + vec3(0.0,   d, 0.0)) - distanceFunc(p + vec3(0.0,  -d, 0.0)),
        distanceFunc(p + vec3(0.0, 0.0,   d)) - distanceFunc(p + vec3(0.0, 0.0,  -d))
    ));
}

void main()
{
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    
    vec3 ray = normalize(vec3(sin(fov) * p.x, sin(fov) * p.y, -cos(fov)));

    float distance = 0.0;
    float rLen = 0.0;
    vec3  rPos = cPos;
    for(int i = 0; i < 64; i++){
        distance = distanceFunc(rPos);
        rLen += distance;
        rPos = cPos + ray * rLen;
    } 

    if(abs(distance) < 0.001)
    {
        vec3 normal = getNormal(rPos);
        float diff = clamp(dot(lightDir, normal), 0.1, 1.0);
        FragColor = vec4(vec3(diff), 1.0);
    }else
    {
        FragColor = vec4(vec3(0.0), 1.0);
    }
}