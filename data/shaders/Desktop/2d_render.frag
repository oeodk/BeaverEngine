#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_texture_sampler;

uniform vec4 u_color;
uniform float u_width;
uniform float u_height;


void main()
{

	FragColor = u_color * texture(u_texture_sampler, texCoord);
}