#version 330 core

in vec2 uv_frag;
uniform sampler2D image_texture;

layout(location = 0) out vec4 FragColor;


// Apply a filter keeping image in the center while fading to black on the border
vec3 filter_color(in vec3 color, in vec2 uv, in float sigma) {

	// Convert position between [-1,1] center around (0,0)
	vec2 p = 2.0 * (uv - vec2(0.5, 0.5));

	// Set a Gaussian weight around (0,0) with std-dev sigma
	float dist = length(p);
	float w = exp(-(dist / sigma) * (dist / sigma));

	// Apply the weighted black filter
	vec3 black = vec3(0.0, 0.0, 0.0);

	return w * color + (1.0 - w) * black;
}

void main()
{
	// Retrieve current color
	vec3 color = texture(image_texture, uv_frag).rgb;

	// Apply first filter: Black & white effect
	color = filter_color(color, uv_frag, 0.8);

	FragColor = vec4(color, 1.0);
}
