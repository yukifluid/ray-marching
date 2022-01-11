#version 330 core

layout (location = 0) in vec3 attribute_pos;

void main()
{
   gl_Position = vec4(attribute_pos, 1.0);
}