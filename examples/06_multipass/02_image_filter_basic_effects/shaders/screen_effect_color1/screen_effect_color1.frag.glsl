#version 330 core

in vec2 uv_frag;
uniform sampler2D image_texture;

layout(location = 0) out vec4 FragColor;



void main()
{
	// Retrieve current color
	vec4 color = texture(image_texture, uv_frag);
	
	// Reduce the number of color
	color = floor(color * 5.0) / 5.0;
	
	FragColor = color;
}
