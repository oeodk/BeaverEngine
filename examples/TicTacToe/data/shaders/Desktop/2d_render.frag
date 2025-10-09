#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 texCoord;
in vec4 color;

uniform sampler2D u_texture_sampler;

uniform float u_width;
uniform float u_height;


void main()
{

	FragColor = color * texture(u_texture_sampler, texCoord);
}