#version 330 core

in vec2 uv_frag;
uniform sampler2D image_texture;

layout(location = 0) out vec4 FragColor;
uniform float time;

// Gold noise from https://www.shadertoy.com/view/ltB3zD
const float PHI = 1.61803398874989484820459;
float gold_noise(in vec2 xy, in float seed){
	return fract(tan(distance(xy * PHI, xy) * seed) * xy.x);
}

// Create a RGB noise using the Gold noise
vec3 rgb_noise(in vec2 uv, in float t) {
	return vec3(gold_noise(uv_frag*5000.0, t+0.1), gold_noise(uv_frag*5000.0, t + 0.2), gold_noise(uv_frag*5000.0, t + 0.3));
}

// Apply a filter keeping only the colored image in the center while being black & white on the border
vec3 filter_color(in vec3 color, in vec2 uv, in float sigma) {
	
	// Convert position between [-1,1] center around (0,0)
	vec2 p = 2.0 * (uv - vec2(0.5, 0.5));

	// Set a Gaussian weight around (0,0) with std-dev sigma
	float dist = length(p);
	float w = exp(-(dist / sigma) * (dist / sigma));

	// Apply the weighted black&white filter
	float black_white = 0.33 * (color.r + color.g + color.b);

	return w * color + (1.0 - w) * black_white * vec3(1.0, 1.0, 1.0);
}

void main()
{
	// Retrieve current color
	vec3 color = texture(image_texture, uv_frag).rgb;
	
	// Apply first filter: Black & white effect
	color = filter_color(color, uv_frag, 0.5);

	// Apply second filter: Noise
	color = color.xyz + 0.2*rgb_noise(uv_frag, time);
	
	FragColor = vec4(color, 1.0);
}
 