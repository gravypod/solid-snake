#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 translation;
uniform mat4 projection;

void main()
{
    gl_Position = (projection * translation) * vec4(aPos, 1.0);
}