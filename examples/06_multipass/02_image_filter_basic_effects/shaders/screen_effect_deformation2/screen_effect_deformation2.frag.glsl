#version 330 core

in vec2 uv_frag;
uniform sampler2D image_texture;

layout(location=0) out vec4 FragColor;

uniform float time;


vec2 deformation(in vec2 uv, in float sigma, in float wavelength, in float frequency, in float t) {

	// Convert position between [-1,1] center around (0,0)
	vec2 p = 2.0 * (uv - vec2(0.5, 0.5));
	// Distance to the center of the screen
	float dist = length(p);

	// Compute weight varying as an increasing & oscillating function of the distance
	float alpha = (dist / sigma) * (dist / sigma) * cos(2*3.14* (dist/wavelength - frequency * time) );

	// The deformation offset is aligned with a vector pointing outward of the center
	vec2 u = p / length(p); // unit vector going away of the center 
	return alpha * u;       // deformation vector

}

void main()
{
	vec2 offset = 0.1*deformation(uv_frag, 1.5, 0.4, 0.8, time);
	vec4 color = texture(image_texture, uv_frag+offset );

	FragColor = color;
}
