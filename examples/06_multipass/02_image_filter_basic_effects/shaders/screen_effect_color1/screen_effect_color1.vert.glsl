#version 330 core

layout (location = 0) in vec4 vertex_position;
layout (location = 3) in vec2 uv;

out vec2 uv_frag;

void main()
{
	uv_frag = uv;
	gl_Position = vertex_position;
}
