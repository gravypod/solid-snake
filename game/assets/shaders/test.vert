#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 translation;
uniform mat4 projection;

uniform float quad_width;
uniform float quad_height;

out vec2 TexCoord;

void main()
{
    gl_Position = (projection * translation) * vec4(aPos, 1.0);

    TexCoord = vec2(aPos.x / quad_width, aPos.y / quad_height);
}