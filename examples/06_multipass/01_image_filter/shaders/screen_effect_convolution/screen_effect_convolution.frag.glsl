#version 330 core

in vec2 uv_frag;
uniform sampler2D image_texture;

layout(location=0) out vec4 FragColor;

// Convolution kernel
float kernel[9] = float[](
	0.0, 1.0, 0.0,
	1.0,-4.0, 1.0,
	0.0, 1.0, 0.0
);

void main()
{

	ivec2 image_size = textureSize(image_texture, 0);
	float dx = 1.0 / float(image_size.x);
	float dy = 1.0 / float(image_size.y);

	vec2 offset[9] = vec2[](
		vec2(-dx, dy), vec2(0.0, dy), vec2(dx, dy),
		vec2(-dx, 0.0), vec2(0.0, 0.0), vec2(dx, 0.0),
		vec2(-dx, -dy), vec2(0.0, -dy), vec2(dx, -dy)
	);


	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = 0; i < 9; i++) {
        color += kernel[i] * texture(image_texture, uv_frag+offset[i] );
    }

	FragColor = abs(1.0-color);
}
