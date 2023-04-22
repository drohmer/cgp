#version 330 core

in vec2 uv_frag;
uniform sampler2D image_texture;

layout(location=0) out vec4 FragColor;


void main()
{
	// Deform the x-coordinate of the texture along a cos function in the y-direction.
	vec2 offset = vec2(0.01 * cos(5 * 3.14 * uv_frag.y), 0.0);
	vec4 color = texture(image_texture, uv_frag+offset);

	FragColor = color;
}
