#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable


// Terrible but only resource: https://www.khronos.org/opengl/wiki/Geometry_Shader
layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

out gl_PerVertex {
    vec4 gl_Position;
};

float triangle_radius = 0.5f;

vec4 triangle_first(vec4 triangle_center_position) {
    return triangle_center_position + vec4(0.0f, -triangle_radius, 0.0f, 0.0f);
}

vec4 triangle_second(vec4 triangle_center_position) {
    return triangle_center_position + vec4(triangle_radius, triangle_radius, 0.0f, 0.0f);
}

vec4 triangle_third(vec4 triangle_center_position) {
    return triangle_center_position + vec4(-triangle_radius, triangle_radius, 0.0f, 0.0f);
}


void main(void)
{

	for(int i = 0; i < gl_in.length(); i++)
	{
	    {
            gl_Position = triangle_first(gl_in[i].gl_Position);
            EmitVertex();

            gl_Position = triangle_second(gl_in[i].gl_Position);
            EmitVertex();

            gl_Position = triangle_third(gl_in[i].gl_Position);
            EmitVertex();
        }
        EndPrimitive();
    }
}