#version 330 core


layout(location=0) out vec4 FragColor;
uniform vec3 color; // Uniform color of the object

void main()
{
	FragColor = vec4(color, 1.0);
}
