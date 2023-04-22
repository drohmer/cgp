#version 330 core

in vec2 uv_frag;
uniform sampler2D image_texture;

layout(location = 0) out vec4 FragColor;



// Simply display the current texture
void main()
{
	// Retrieve current color
	vec3 color = texture(image_texture, uv_frag).rgb;

	FragColor = vec4(color, 1.0);
}
