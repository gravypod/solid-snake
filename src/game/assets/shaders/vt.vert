#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
    vec4 gl_Position;
};

//vec2 positions[3] = vec2[](
//    vec2(0.0, -0.5),
//    vec2(0.5, 0.5),
//    vec2(-0.5, 0.5)
//);

layout(location = 0) in vec2 triangle_center_pos;

void main() {
    gl_Position = vec4(triangle_center_pos, 0.0, 1.0);
}