#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform float u_width;
uniform float u_height;
uniform mat4 u_projection_view;

void main()
{
	texCoord = inTexCoord;
    gl_Position = u_projection_view * vec4(inPosition, 1.0);
}