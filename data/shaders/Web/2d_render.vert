#version 300 es
precision mediump float;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inColor;

out vec2 texCoord;
out vec4 color;

uniform float u_width;
uniform float u_height;
uniform mat4 u_projection_view;

void main()
{
	color = inColor;
	texCoord = inTexCoord;
    gl_Position = u_projection_view * vec4(inPosition, 1.0);
}