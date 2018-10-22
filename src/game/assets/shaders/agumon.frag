#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texture;

layout(location = 0) in vec3 textureNormal;
layout(location = 1) in vec2 textureCoord;

layout(location = 0) out vec4 color;


void main() {
    color = texture(texSampler, fragTexCoord);
}