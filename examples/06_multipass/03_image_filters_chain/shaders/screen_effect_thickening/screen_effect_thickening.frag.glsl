#version 330 core

in vec2 uv_frag;
uniform sampler2D image_texture;

layout(location=0) out vec4 FragColor;


uniform int thickness;

void main()
{

	ivec2 image_size = textureSize(image_texture, 0);
	float dx = 1.0 / float(image_size.x);
	float dy = 1.0 / float(image_size.y);


	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	for (int kx = -thickness; kx < thickness; kx++) {
		for (int ky = -thickness; ky < thickness; ky++) {
			color = min(color, texture(image_texture, uv_frag + vec2(float(kx)*dx,float(ky)*dy)));
		}
	}

	FragColor = color;
}
