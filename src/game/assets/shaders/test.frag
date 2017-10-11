#version 330 core

out vec4 FragColor;
 in vec2 TexCoord;

uniform sampler2D test_texture;

void main()
{
	FragColor = texture(test_texture, TexCoord);
}